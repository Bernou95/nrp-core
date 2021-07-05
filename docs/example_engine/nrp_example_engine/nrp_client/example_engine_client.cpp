#include "nrp_example_engine/nrp_client/example_engine_client.h"

ExampleEngineClient::~ExampleEngineClient()
{}

void ExampleEngineClient::initialize()
{

}

void ExampleEngineClient::shutdown()
{

}

SimulationTime ExampleEngineClient::sendRunLoopStepCommand(SimulationTime timeStep)
{
<<<<<<< HEAD
    (void)timeStep;
=======
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
    return SimulationTime::zero();
}

void ExampleEngineClient::sendDevicesToEngine(const devices_ptr_t &devicesArray)
{
<<<<<<< HEAD
    (void)devicesArray;
=======

>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
}

EngineClientInterface::devices_set_t ExampleEngineClient::getDevicesFromEngine(const device_identifiers_set_t &deviceIdentifiers)
{
<<<<<<< HEAD
    (void)deviceIdentifiers;
=======
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
    return EngineClientInterface::devices_set_t();
}

const std::vector<std::string> ExampleEngineClient::engineProcStartParams() const
{
    return std::vector<std::string>();
}

const std::vector<std::string> ExampleEngineClient::engineProcEnvParams() const
{
    return std::vector<std::string>();
}
