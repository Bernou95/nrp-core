
#include <std_msgs/ColorRGBA.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const ColorRGBA& m1, const ColorRGBA& m2)
{
  return false;
}

void exportColorRGBA ()
{
  using std_msgs::ColorRGBA;
  class_<ColorRGBA, shared_ptr<ColorRGBA> >("ColorRGBA", "float32 r\nfloat32 g\nfloat32 b\nfloat32 a\n")
    .def_readwrite("r", &ColorRGBA::r)
    .def_readwrite("g", &ColorRGBA::g)
    .def_readwrite("b", &ColorRGBA::b)
    .def_readwrite("a", &ColorRGBA::a)
    ;

}

} // namespace