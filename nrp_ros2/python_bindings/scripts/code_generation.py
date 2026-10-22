# NRP Core - Backend infrastructure to synchronize simulations
#
# Copyright 2020-2023 NRP Team
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# This project has received funding from the European Union’s Horizon 2020
# Framework Programme for Research and Innovation under the Specific Grant
# Agreement No. 945539 (Human Brain Project SGA3).
#
# This file is a modified version of another one originally authored by:
# Bhaskara Marthi with license BSD.
# The original file can be found at: https://github.com/bhaskara/boost_python_ros
#
# ROS 2 version of the ROS 1 nrp_ros/python_bindings/scripts/code_generation.py (removed). Messages are read with rosidl_adapter
# from the packages installed in the ament index. Bindings are generated for the C++ message structs created
# by rosidl_generator_cpp (pkg::msg::Name).

from io import StringIO
import os

from ament_index_python import get_resource
from ament_index_python.packages import get_package_share_directory
from rosidl_adapter.parser import parse_message_file
from rosidl_pycommon import convert_camel_case_to_lower_case_underscore

############################################################
# Main functions
############################################################

PYTHON_PACKAGE = "nrp_core.data.nrp_ros"


def generate_export_function(pkg, msg, spec, s):
    "Generate the single helper function that exports the definition of a message type."

    s.write("void export{0} ()".format(msg))
    s.write("{")
    with Indent(s, 2):
        s.write("using {0}::msg::{1};".format(pkg, msg))
        s.write('DataPack<{0}>::create_python("{0}DataPack");'.format(msg))
        s.write('class_<vector<const {0}*> >("{0}Vector").def(vector_indexing_suite<vector<const {0}*> >());'
                .format(msg))
        s.write('class_<{0}, std::shared_ptr<{0}> > cls("{0}", "{1}");'.format(msg, class_docstring(pkg, msg)))
        s.write("cls")

        with Indent(s, 2):
            for field in spec.fields:
                s.write('.def_readwrite("{0}", &{1}::{0})'.format(field.name, msg))
            s.write('.def("__str__", &nrp_ros2::toYaml<{0}>);\n'.format(msg))

        for c in spec.constants:
            if c.type in MSG_TYPE_TO_CPP and c.type not in ('string', 'wstring'):
                s.write('cls.attr("{0}") = static_cast<{1}>({2}::{0});'.format(c.name, python_int_type(c.type), msg))

        s.write("register_ptr_to_python<const {0} *>();\n".format(msg))

        for f in spec.fields:
            t = f.type
            if not t.is_array:
                continue

            if t.is_upper_bound:
                raise ValueError("Field '{0}' of message '{1}/{2}': bounded sequences are not supported"
                                 .format(f.name, pkg, msg))

            if t.array_size:
                s.write('nrp_ros2::exposeArray<{0}, {1}>("{2}_{3}");'.format(cpp_base_type(t), t.array_size, msg, f.name))
            else:
                s.write('nrp_ros2::exposeVector<{0}>("{1}_{2}");'.format(cpp_base_type(t), msg, f.name))
    s.write("}\n")


def generate_file(pkg, msg, s=None):
    "Generate the definition file for a single message"

    if s is None:
        s = IndentedWriter()
    spec = get_msg_spec(pkg, msg)

    s.write("#include <{0}>".format(msg_header(pkg, msg)))
    s.write("#include <boost/python.hpp>")
    s.write("#include <boost/python/suite/indexing/vector_indexing_suite.hpp>")
    s.write("#include \"nrp_general_library/datapack_interface/datapack.h\"")
    s.write("#include \"python_bindings/ros2_msg_bindings.h\"\n")
    s.write("\n")

    s.write("namespace {0}".format(pkg))
    s.write("{")
    s.write("\n")

    s.write("using namespace boost::python;")
    s.write("using std::vector;")
    s.write("\n")

    generate_export_function(pkg, msg, spec, s)

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
    messages = list_msg_types(pkg)
    for m in messages:
        s.write("void export{0}();".format(m))

    # Boost python wrappers
    s.write("\n")
    s.write("BOOST_PYTHON_MODULE({0})".format(pkg))
    s.write("{")
    with Indent(s, 2):
        # Types from other packages used in fields must be registered first
        for dep in sorted(package_dependencies(pkg)):
            s.write('boost::python::import("{0}.{1}");'.format(PYTHON_PACKAGE, dep))
        for m in messages:
            s.write("export{0}();".format(m))
    s.write("}\n")

    s.write("} // namespace")
    return s.getvalue()


def generate_rclpy_conversion(pkg, msg, s=None):
    if s is None:
        s = IndentedWriter()

    spec = get_msg_spec(pkg, msg)
    s.write("def {0}_to_ros(x):".format(msg))
    with Indent(s, 4):
        s.write("try:")
        with Indent(s, 4):
            s.write("m = {0}.msg.{1}()".format(pkg, msg))
            for f in spec.fields:
                t = f.type
                e = "x.{0}".format(f.name)
                if t.is_primitive_type():
                    if t.is_array and t.type in ('byte', 'uint8', 'char'):
                        s.write("m.{0} = bytes({1})".format(f.name, e) if t.type != 'char' else
                                "m.{0} = list({1})".format(f.name, e))
                    elif t.is_array:
                        s.write("m.{0} = list({1})".format(f.name, e))
                    else:
                        s.write("m.{0} = {1}".format(f.name, e))
                else:
                    conv = "{0}_to_ros".format(t.type)
                    if t.is_array:
                        s.write("m.{0} = [{1}(e) for e in {2}]".format(f.name, conv, e))
                    else:
                        s.write("m.{0} = {1}({2})".format(f.name, conv, e))

            s.write("return m")

        s.write("except Exception as e:")
        with Indent(s, 4):
            s.write("if not isinstance(x, cpp.{0}):".format(msg))
            with Indent(s, 4):
                error_msg = "\"'{0}_to_ros' must be called with objects of type '{2}.{1}.{0}', " \
                            "called with object of type '{{}}'\"".format(msg, pkg, PYTHON_PACKAGE)
                s.write("raise TypeError({}.format(type(x)))".format(error_msg))
            s.write("else:")
            with Indent(s, 4):
                s.write("raise e\n")

    s.write("def {0}_to_boost(x):".format(msg))
    with Indent(s, 4):
        s.write("try:")
        with Indent(s, 4):
            s.write("m = cpp.{0}()".format(msg))
            for f in spec.fields:
                t = f.type
                e = "x.{0}".format(f.name)
                if t.is_primitive_type():
                    # rclpy returns byte values as bytes objects of length 1
                    if t.is_array and t.type == 'byte':
                        s.write("m.{0} = [_byte_to_int(e) for e in {1}]".format(f.name, e))
                    elif t.is_array:
                        s.write("m.{0} = list({1})".format(f.name, e))
                    elif t.type == 'byte':
                        s.write("m.{0} = _byte_to_int({1})".format(f.name, e))
                    else:
                        s.write("m.{0} = {1}".format(f.name, e))
                else:
                    conv = "{0}_to_boost".format(t.type)
                    if t.is_array:
                        s.write("for e in {0}:".format(e))
                        with Indent(s, 4):
                            s.write("m.{0}.append({1}(e))".format(f.name, conv))
                    else:
                        s.write("m.{0} = {1}({2})".format(f.name, conv, e))

            s.write("return m")

        s.write("except Exception as e:")
        with Indent(s, 4):
            s.write("if not isinstance(x, {0}.msg.{1}):".format(pkg, msg))
            with Indent(s, 4):
                error_msg = "\"'{0}_to_boost' must be called with objects of type '{1}.msg.{0}', " \
                            "called with object of type '{{}}'\"".format(msg, pkg)
                s.write("raise TypeError({}.format(type(x)))".format(error_msg))
            s.write("else:")
            with Indent(s, 4):
                s.write("raise e\n")

    return s.getvalue()


def generate_python_imports(pkg):
    return '\n'.join("from {0}.{1}_conversions import *".format(PYTHON_PACKAGE, p)
                     for p in sorted(package_dependencies(pkg)))


def write_rclpy_conversions(pkg, target_dir):
    """
    Generate all rclpy conversions.
    @param pkg: We'll generate conversion functions for all messages defined in this ros package.
    """
    outfile = os.path.join(target_dir, pkg+'_conversions.py')
    with open(outfile, 'w') as f:
        f.write(generate_python_imports(pkg))
        f.write("\nimport {0}.msg\n".format(pkg))
        f.write("import {0}.{1} as cpp\n".format(PYTHON_PACKAGE, pkg))
        f.write("\n\ndef _byte_to_int(b):\n    return b if isinstance(b, int) else b[0]\n\n")

        for m in list_msg_types(pkg):
            f.write(generate_rclpy_conversion(pkg, m))


def write_bindings(pkg, target_dir):
    "Generate and write all bindings"
    top_level_file = os.path.join(target_dir, pkg+'.cpp')
    with open(top_level_file, 'w') as f:
        f.write(generate_package_file(pkg))
    for m in list_msg_types(pkg):
        outfile = os.path.join(target_dir, "{0}_{1}.cpp".format(pkg, m))
        with open(outfile, 'w') as f:
            f.write(generate_file(pkg, m))


def write_msg_lists(pkg, types_file, headers_file):
    "Append the C++ message types and headers of the package, used to configure the ROS 2 event loop nodes"
    with open(types_file, 'a') as f:
        f.write(''.join("{0}::msg::{1};".format(pkg, m) for m in list_msg_types(pkg)))
    with open(headers_file, 'a') as f:
        f.write(''.join("{0};".format(msg_header(pkg, m)) for m in list_msg_types(pkg)))


############################################################
# Helpers
############################################################

MSG_TYPE_TO_CPP = {'byte': 'uint8_t', 'char': 'uint8_t',
                   'bool': 'bool',
                   'uint8': 'uint8_t', 'int8': 'int8_t',
                   'uint16': 'uint16_t', 'int16': 'int16_t',
                   'uint32': 'uint32_t', 'int32': 'int32_t',
                   'uint64': 'uint64_t', 'int64': 'int64_t',
                   'float32': 'float',
                   'float64': 'double',
                   'string': 'std::string',
                   'wstring': 'std::u16string'}


def python_int_type(msg_type):
    "Type used to export numeric constants. 8 bit integers would otherwise be exported as characters"
    return {'byte': 'int', 'char': 'int', 'uint8': 'int', 'int8': 'int'}.get(msg_type, MSG_TYPE_TO_CPP[msg_type])


def cpp_base_type(t):
    if t.is_primitive_type():
        if t.type == 'wstring':
            raise ValueError("wstring fields are not supported")
        return MSG_TYPE_TO_CPP[t.type]
    return "{0}::msg::{1}".format(t.pkg_name, t.type)


def msg_header(pkg, msg):
    return "{0}/msg/{1}.hpp".format(pkg, convert_camel_case_to_lower_case_underscore(msg))


def list_msg_types(pkg):
    content, _ = get_resource('rosidl_interfaces', pkg)
    return sorted(os.path.splitext(os.path.basename(l))[0] for l in content.splitlines()
                  if l.startswith('msg/') and l.endswith('.msg'))


_SPECS = {}


def get_msg_spec(pkg, msg):
    if (pkg, msg) not in _SPECS:
        path = os.path.join(get_package_share_directory(pkg), 'msg', msg + '.msg')
        _SPECS[(pkg, msg)] = parse_message_file(pkg, path)
    return _SPECS[(pkg, msg)]


def package_dependencies(pkg):
    "Packages, other than pkg, of the message types used in the fields of the messages of pkg"
    deps = set()
    for m in list_msg_types(pkg):
        for f in get_msg_spec(pkg, m).fields:
            if not f.type.is_primitive_type() and f.type.pkg_name != pkg:
                deps.add(f.type.pkg_name)
    return deps


def class_docstring(pkg, msg):
    path = os.path.join(get_package_share_directory(pkg), 'msg', msg + '.msg')
    with open(path) as f:
        text = f.read()
    return text.replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n')


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
