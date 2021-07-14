# Software License Agreement (BSD License)
#
# Copyright (c) 2008, Willow Garage, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above
#    copyright notice, this list of conditions and the following
#    disclaimer in the documentation and/or other materials provided
#    with the distribution.
#  * Neither the name of Willow Garage, Inc. nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# Author: Bhaskara Marthi

# Generation of Boost python bindings for ROS messages

#from cStringIO import StringIO
from io import StringIO
from roslib import packages
from roslib import msgs
from roslib import rospack
import os
import re
import itertools as it

############################################################
# Main functions
############################################################


def generate_export_function(spec, s):
    "Generate the single helper function that exports the definition of a message type."

    msg = spec.short_name
    s.write("void export{0} ()".format(msg))
    s.write("{")
    with Indent(s, 2):
        s.write("using {0}::{1};".format(spec.package, msg))
        s.write('class_<{0}, shared_ptr<{0}> >("{0}", "{1}")'.\
                format(msg, class_docstring(spec)))

        array_fields = []

        with Indent(s, 2):
            for field in spec.parsed_fields():
                s.write('.def_readwrite("{0}", &{1}::{0})'.format(field.name, msg))
                if field.is_array:
                    array_fields.append(field)
            s.write(';\n')

        for f in array_fields:
            s.write('class_<vector<{0}> >("{1}_{2}")'.format(qualify(f.base_type),
                                                               msg, f.name))
            with Indent(s, 2):
                s.write('.def(vector_indexing_suite<vector<{0}> > ())'.format(qualify(f.base_type)))
                s.write(';')
    s.write("}\n")


def generate_equality_forward_declarations(spec, s):
    already_seen = set()
    for f in spec.parsed_fields():
        if f.is_array and not f.is_builtin:
            if f.base_type in already_seen:
                continue
            already_seen.add(f.base_type)
            m = re.match('(\w+)/', f.base_type)
            pkg = m.group(1)
            s.write("namespace {0}".format(pkg))
            s.write("\n{")
            s.write("inline bool operator== (const {0}& m1, const {0}& m2)".
                    format(qualify(f.base_type)))
            s.write("{\n  return false;\n}\n\n} // namespace")
            #s.write("namespace {0}".format(pkg))
            #s.write("{\n")
            #s.write("bool operator== (const {0}& m1, const {0}& m2);\n".\
            #        format(qualify(f.base_type)))
            #s.write("}\n")

def generate_file(pkg, msg, s=None):
    "Generate the definition file for a single message"
    
    if s is None:
        s = IndentedWriter()
    spec = get_msg_spec(pkg, msg)

    s.write("#include <{0}/{1}.h>".format(pkg, msg))
    s.write("#include <boost/python.hpp>")
    s.write("#include <boost/python/suite/indexing/vector_indexing_suite.hpp>")
    s.write("\n")

    generate_equality_forward_declarations(spec, s)


    s.write("namespace {0}".format(pkg))
    s.write("{")
    s.write("\n")

    s.write("using namespace boost::python;")
    s.write("using boost::shared_ptr;")
    s.write("using std::vector;")
    s.write("\n")

    
    s.write("// Dummy equality check to avoid compilation error for vector_indexing_suite")
    s.write("bool operator== (const {0}& m1, const {0}& m2)".\
            format(msg))
    s.write("{")
    with Indent(s, 2):
        s.write("return false;")
    s.write("}\n")


    generate_export_function(spec, s)

    s.write("} // namespace")

    return s.getvalue()


def generate_package_file(pkg, s=None):
    "Generate the top level file for the given package with forward declarations for individual messages"
    if s is None:
        s = IndentedWriter()

    # Header
    s.write("#include <boost/python.hpp>\n")
    s.write("namespace {0}".format(pkg))
    s.write("{\n")

    # Forward declarations
    messages = msgs.list_msg_types(pkg, False)
    for m in messages:
        s.write("void export{0}();".format(m));

    # Boost python wrappers
    s.write("\n")
    s.write("BOOST_PYTHON_MODULE({0}_cpp)".format(pkg))
    s.write("{")
    with Indent(s, 2):
        for m in messages:
            s.write("export{0}();".format(m))
    s.write("}\n")

    s.write("} // namespace")
    return s.getvalue()

def generate_rospy_conversion(pkg, msg, s=None):
    if s is None:
        s = IndentedWriter()

    spec = get_msg_spec(pkg, msg)
    s.write("def {0}_to_ros(x):".format(msg))
    with Indent(s, 4):
        s.write("m = {0}.msg.{1}()".format(pkg, msg))
        for f in spec.parsed_fields():
            if f.type in ['time', 'duration']:
                 s.write("m.{0}.secs = x.{0}.sec".format(f.name))
                 s.write("m.{0}.nsecs = x.{0}.nsec".format(f.name))
            else:
                if f.is_builtin:
                    conv = "m.{0} = x.{0}"
                elif f.is_array:
                    conv = "m.{0} = [y.to_ros() for y in x.{0}]"
                else:
                    conv = "m.{0} = x.{0}.to_ros()"
                s.write(conv.format(f.name))
        s.write("return m\n")
        
    s.write("cpp.{0}.to_ros = {0}_to_ros\n".format(msg))

    s.write("def {0}_to_boost(m):".format(msg))
    with Indent(s, 4):
        s.write("x = cpp.{0}()".format(msg))
        for f in spec.parsed_fields():
            if f.type in ['time', 'duration']:
                s.write("x.{0}.sec = m.{0}.secs".format(f.name))
                s.write("x.{0}.nsec = m.{0}.nsecs".format(f.name))
            else:
                if f.is_builtin:
                    conv = "x.{0} = m.{0}"
                elif f.is_array:
                    conv = "x.{0}[:] = [y.to_boost() for y in m.{0}]"
                else:
                    conv = "x.{0} = m.{0}.to_boost()"
                s.write(conv.format(f.name))
        s.write("return x\n")

    s.write("{1}.msg.{0}.to_boost = {0}_to_boost\n".format(msg, pkg))
    
    return s.getvalue()


def generate_python_imports(pkg):
    pkgs = set()
    for m in msgs.list_msg_types(pkg, False):
        spec = get_msg_spec(pkg, m)
        for f in spec.parsed_fields():
            if f.is_header:
                pkgs.add('std_msgs')
            else:
                match = re.match('(\w+)/', f.base_type)
                if match and match.group(1)!=pkg:
                    pkgs.add(match.group(1))
    return '\n'.join("import {0}_boost".format(p) for p in pkgs)
                
            
            
        
            
    

def write_rospy_conversions(pkg, target_dir, current_pkg):
    """
    Generate all rospy conversions.
    @param pkg: We'll generate conversion functions for all messages defined in this ros package.
    @param current_pkg: The generated functions are being put into this ros package.
    """
    outfile = os.path.join(target_dir, pkg+'_boost.py')
    with open(outfile, 'w') as f:
        f.write("import roslib; roslib.load_manifest('{0}')\n".format(current_pkg))
        f.write("import rostime_boost_python\n")
        f.write(generate_python_imports(pkg))
        f.write("\nimport {0}.msg\n".format(pkg))
        f.write("import {0}_cpp as cpp\n".format(pkg))


        for m in msgs.list_msg_types(pkg, False):
            f.write(generate_rospy_conversion(pkg, m))
        

def write_bindings(pkg, target_dir):
    "Generate and write all bindings"
    #print(pkg)
    #print(target_dir)
    top_level_file = os.path.join(target_dir, pkg+'.cpp')
    with open(top_level_file, 'w') as f:
        f.write(generate_package_file(pkg))
    for m in msgs.list_msg_types(pkg, False):
        outfile = os.path.join(target_dir, "{0}_{1}.cpp".format(pkg, m))
        with open(outfile, 'w') as f:
            f.write(generate_file(pkg, m))
                  
    
    
    
                
############################################################
# Helpers
############################################################

MSG_TYPE_TO_CPP = {'byte': 'int8_t', 'char': 'uint8_t',
                   'bool': 'uint8_t',
                   'uint8': 'uint8_t', 'int8': 'int8_t', 
                   'uint16': 'uint16_t', 'int16': 'int16_t', 
                   'uint32': 'uint32_t', 'int32': 'int32_t',
                   'uint64': 'uint64_t', 'int64': 'int64_t',
                   'float32': 'float',
                   'float64': 'double',
                   'string': 'std::string',
                   'time': 'ros::Time',
                   'duration': 'ros::Duration'}

    
def qualify(name):
    if '/' in name:
        return name.replace('/', '::')
    else:
        return MSG_TYPE_TO_CPP[name]

def get_msg_spec(pkg, msg):
    path = rospack.rospackexec(['find', pkg])
    return msgs.load_from_file("{0}/msg/{1}.msg".format(path, msg), pkg)[1]

def class_docstring(spec):
    return spec.text.replace('"', '\\"').replace('\n', '\\n')
    

############################################################
# Indented writer
############################################################

class IndentedWriter():

    def __init__(self, s=None):
        self.str = s or StringIO()
        self.indentation = 0
        self.block_indent = False

    def write(self, s, indent=True, newline=True):
        if not indent:
            newline = False
        if self.block_indent:
            self.block_indent = False
        else:
            if newline:
                self.str.write('\n')
            if indent:
                for i in range(self.indentation):
                    self.str.write(' ')
        self.str.write(s)

    def newline(self):
        self.str.write('\n')

    def inc_indent(self, inc=2):
        self.indentation += inc

    def dec_indent(self, dec=2):
        self.indentation -= dec

    def reset_indent(self):
        self.indentation = 0

    def block_next_indent(self):
        self.block_indent = True

    def getvalue(self):
        return self.str.getvalue()

class Indent():

    def __init__(self, w, inc=2, indent_first=True):
        self.writer = w
        self.inc = inc
        self.indent_first = indent_first

    def __enter__(self):
        self.writer.inc_indent(self.inc)
        if not self.indent_first:
            self.writer.block_next_indent()

    def __exit__(self, type, val, traceback):
        self.writer.dec_indent(self.inc)



