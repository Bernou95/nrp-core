
#include <sensor_msgs/TimeReference.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const TimeReference& m1, const TimeReference& m2)
{
  return false;
}

void exportTimeReference ()
{
  using sensor_msgs::TimeReference;
  class_<TimeReference, shared_ptr<TimeReference> >("TimeReference", "# Measurement from an external time source not actively synchronized with the system clock.\n\nHeader header    # stamp is system time for which measurement was valid\n                 # frame_id is not used \n\ntime   time_ref  # corresponding time from this external source\nstring source    # (optional) name of time source\n")
    .def_readwrite("header", &TimeReference::header)
    .def_readwrite("time_ref", &TimeReference::time_ref)
    .def_readwrite("source", &TimeReference::source)
    ;

}

} // namespace