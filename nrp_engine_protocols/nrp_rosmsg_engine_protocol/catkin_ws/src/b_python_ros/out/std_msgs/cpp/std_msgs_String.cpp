
#include <std_msgs/String.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const String& m1, const String& m2)
{
  return false;
}

void exportString ()
{
  using std_msgs::String;
  class_<String, shared_ptr<String> >("String", "string data\n")
    .def_readwrite("data", &String::data)
    ;

}

} // namespace