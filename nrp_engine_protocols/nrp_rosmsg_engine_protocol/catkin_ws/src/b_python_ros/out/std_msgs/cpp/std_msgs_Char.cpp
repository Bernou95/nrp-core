
#include <std_msgs/Char.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Char& m1, const Char& m2)
{
  return false;
}

void exportChar ()
{
  using std_msgs::Char;
  class_<Char, shared_ptr<Char> >("Char", "char data")
    .def_readwrite("data", &Char::data)
    ;

}

} // namespace