
#include <std_msgs/Header.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace std_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const Header& m1, const Header& m2)
{
  return false;
}

void exportHeader ()
{
  using std_msgs::Header;
  class_<Header, shared_ptr<Header> >("Header", "# Standard metadata for higher-level stamped data types.\n# This is generally used to communicate timestamped data \n# in a particular coordinate frame.\n# \n# sequence ID: consecutively increasing ID \nuint32 seq\n#Two-integer timestamp that is expressed as:\n# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n# time-handling sugar is provided by the client library\ntime stamp\n#Frame this data is associated with\nstring frame_id\n")
    .def_readwrite("seq", &Header::seq)
    .def_readwrite("stamp", &Header::stamp)
    .def_readwrite("frame_id", &Header::frame_id)
    ;

}

} // namespace