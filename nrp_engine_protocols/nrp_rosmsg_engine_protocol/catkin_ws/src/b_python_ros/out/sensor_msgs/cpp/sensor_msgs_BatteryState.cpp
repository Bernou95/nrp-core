
#include <sensor_msgs/BatteryState.h>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>


namespace sensor_msgs
{


using namespace boost::python;
using boost::shared_ptr;
using std::vector;


// Dummy equality check to avoid compilation error for vector_indexing_suite
bool operator== (const BatteryState& m1, const BatteryState& m2)
{
  return false;
}

void exportBatteryState ()
{
  using sensor_msgs::BatteryState;
  class_<BatteryState, shared_ptr<BatteryState> >("BatteryState", "\n# Constants are chosen to match the enums in the linux kernel\n# defined in include/linux/power_supply.h as of version 3.7\n# The one difference is for style reasons the constants are\n# all uppercase not mixed case.\n\n# Power supply status constants\nuint8 POWER_SUPPLY_STATUS_UNKNOWN = 0\nuint8 POWER_SUPPLY_STATUS_CHARGING = 1\nuint8 POWER_SUPPLY_STATUS_DISCHARGING = 2\nuint8 POWER_SUPPLY_STATUS_NOT_CHARGING = 3\nuint8 POWER_SUPPLY_STATUS_FULL = 4\n\n# Power supply health constants\nuint8 POWER_SUPPLY_HEALTH_UNKNOWN = 0\nuint8 POWER_SUPPLY_HEALTH_GOOD = 1\nuint8 POWER_SUPPLY_HEALTH_OVERHEAT = 2\nuint8 POWER_SUPPLY_HEALTH_DEAD = 3\nuint8 POWER_SUPPLY_HEALTH_OVERVOLTAGE = 4\nuint8 POWER_SUPPLY_HEALTH_UNSPEC_FAILURE = 5\nuint8 POWER_SUPPLY_HEALTH_COLD = 6\nuint8 POWER_SUPPLY_HEALTH_WATCHDOG_TIMER_EXPIRE = 7\nuint8 POWER_SUPPLY_HEALTH_SAFETY_TIMER_EXPIRE = 8\n\n# Power supply technology (chemistry) constants\nuint8 POWER_SUPPLY_TECHNOLOGY_UNKNOWN = 0\nuint8 POWER_SUPPLY_TECHNOLOGY_NIMH = 1\nuint8 POWER_SUPPLY_TECHNOLOGY_LION = 2\nuint8 POWER_SUPPLY_TECHNOLOGY_LIPO = 3\nuint8 POWER_SUPPLY_TECHNOLOGY_LIFE = 4\nuint8 POWER_SUPPLY_TECHNOLOGY_NICD = 5\nuint8 POWER_SUPPLY_TECHNOLOGY_LIMN = 6\n\nHeader  header\nfloat32 voltage          # Voltage in Volts (Mandatory)\nfloat32 temperature      # Temperature in Degrees Celsius (If unmeasured NaN)\nfloat32 current          # Negative when discharging (A)  (If unmeasured NaN)\nfloat32 charge           # Current charge in Ah  (If unmeasured NaN)\nfloat32 capacity         # Capacity in Ah (last full capacity)  (If unmeasured NaN)\nfloat32 design_capacity  # Capacity in Ah (design capacity)  (If unmeasured NaN)\nfloat32 percentage       # Charge percentage on 0 to 1 range  (If unmeasured NaN)\nuint8   power_supply_status     # The charging status as reported. Values defined above\nuint8   power_supply_health     # The battery health metric. Values defined above\nuint8   power_supply_technology # The battery chemistry. Values defined above\nbool    present          # True if the battery is present\n\nfloat32[] cell_voltage   # An array of individual cell voltages for each cell in the pack\n                         # If individual voltages unknown but number of cells known set each to NaN\nfloat32[] cell_temperature  # An array of individual cell temperatures for each cell in the pack\n                            # If individual temperatures unknown but number of cells known set each to NaN\nstring location          # The location into which the battery is inserted. (slot number or plug)\nstring serial_number     # The best approximation of the battery serial number\n")
    .def_readwrite("header", &BatteryState::header)
    .def_readwrite("voltage", &BatteryState::voltage)
    .def_readwrite("temperature", &BatteryState::temperature)
    .def_readwrite("current", &BatteryState::current)
    .def_readwrite("charge", &BatteryState::charge)
    .def_readwrite("capacity", &BatteryState::capacity)
    .def_readwrite("design_capacity", &BatteryState::design_capacity)
    .def_readwrite("percentage", &BatteryState::percentage)
    .def_readwrite("power_supply_status", &BatteryState::power_supply_status)
    .def_readwrite("power_supply_health", &BatteryState::power_supply_health)
    .def_readwrite("power_supply_technology", &BatteryState::power_supply_technology)
    .def_readwrite("present", &BatteryState::present)
    .def_readwrite("cell_voltage", &BatteryState::cell_voltage)
    .def_readwrite("cell_temperature", &BatteryState::cell_temperature)
    .def_readwrite("location", &BatteryState::location)
    .def_readwrite("serial_number", &BatteryState::serial_number)
    ;

  class_<vector<float> >("BatteryState_cell_voltage")
    .def(vector_indexing_suite<vector<float> > ())
    ;
  class_<vector<float> >("BatteryState_cell_temperature")
    .def(vector_indexing_suite<vector<float> > ())
    ;
}

} // namespace