//
// NRP Core - Backend infrastructure to synchronize simulations
//
// Copyright 2020-2021 NRP Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This project has received funding from the European Union’s Horizon 2020
// Framework Programme for Research and Innovation under the Specific Grant
// Agreement No. 945539 (Human Brain Project SGA3).
//

#include <iostream>

#include <gtest/gtest.h>

#include "nrp_general_library/process_launchers/process_launcher_basic.h"
#include "nrp_grpc_engine_protocol/config/engine_grpc_config.h"
<<<<<<< HEAD
=======
#include "nrp_grpc_engine_protocol/device_interfaces/grpc_device_serializer.h"
#include "nrp_grpc_engine_protocol/engine_server/engine_grpc_device_controller.h"
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
#include "nrp_grpc_engine_protocol/engine_server/engine_grpc_server.h"
#include "nrp_grpc_engine_protocol/engine_client/engine_grpc_client.h"
#include "nrp_grpc_engine_protocol/grpc_server/engine_grpc.grpc.pb.h"


void testSleep(unsigned sleepMs)
{
    std::chrono::milliseconds timespan(sleepMs);
    std::this_thread::sleep_for(timespan);
}

<<<<<<< HEAD

class TestGrpcDeviceController
        : public DataDeviceController<google::protobuf::Message>
{
    public:

		TestGrpcDeviceController()
		    : _data(new EngineGrpc::TestPayload())
        { }

        virtual void handleDeviceData(const google::protobuf::Message &data) override
		{
            // throws bad_cast
            const auto &j = dynamic_cast<const EngineGrpc::TestPayload &>(data);
		    _data->CopyFrom(j);
		}

        virtual google::protobuf::Message *getDeviceInformation() override
		{
            if(this->_returnEmptyDevice)
                return nullptr;
            else {
                auto old_data = _data;
                _data = new EngineGrpc::TestPayload();
                _data->CopyFrom(*old_data);
                return old_data;
            }
=======
class TestGrpcDeviceInterface1
    : public Device<TestGrpcDeviceInterface1, "test_type1", PropNames<"testProperty">, uint32_t >
{
	public:
		TestGrpcDeviceInterface1(DeviceIdentifier &&devID, property_template_t &&props = property_template_t())
		    : Device(std::move(devID), std::move(props))
		{}

    uint32_t getTestProperty() const
    {
        return this->getPropertyByName<"testProperty">();
    }

    void setTestProperty(const uint32_t value)
    {
        this->getPropertyByName<"testProperty">() = value;
    }
};

template<>
GRPCDevice DeviceSerializerMethods<GRPCDevice>::serialize<TestGrpcDeviceInterface1>(const TestGrpcDeviceInterface1 &device)
{
    // Copy device ID to the gRPC message

    GRPCDevice grpcMessage = serializeID<GRPCDevice>(device.id());

    // Get pointer to the message data

    auto myDevice = grpcMessage.dev().mutable_test();

    myDevice->InitAsDefaultInstance();

    // Copy data from scalar property to the gRPC message

    myDevice->set_integer(device.getTestProperty());

    return grpcMessage;
}

template<>
TestGrpcDeviceInterface1 DeviceSerializerMethods<GRPCDevice>::deserialize<TestGrpcDeviceInterface1>(DeviceIdentifier &&devID, deserialization_t data)
{
    return TestGrpcDeviceInterface1(std::move(devID), TestGrpcDeviceInterface1::property_template_t(data->test().integer()));
}


class TestGrpcDeviceController
        : public EngineGrpcDeviceController<TestGrpcDeviceInterface1>
{
    public:

		TestGrpcDeviceController(DeviceIdentifier &&devID)
		    : EngineGrpcDeviceController<TestGrpcDeviceInterface1>(std::move(devID)),
		      _dev(DeviceIdentifier(*this))
		{}

		virtual void handleDeviceDataCallback(TestGrpcDeviceInterface1 &&data) override
		{	this->_dev = std::move(data);	}

		virtual const TestGrpcDeviceInterface1 *getDeviceInformationCallback() override
		{
            return this->_returnEmptyDevice ? nullptr : &this->_dev;
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
        }

        void triggerEmptyDeviceReturn(bool value)
        {
            this->_returnEmptyDevice = value;
        }

	private:
<<<<<<< HEAD
        EngineGrpc::TestPayload* _data;
=======
		TestGrpcDeviceInterface1 _dev;
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
        bool _returnEmptyDevice = false;
};

struct TestEngineGRPCConfigConst
{
    static constexpr FixedString EngineType = "test_engine";
    static constexpr FixedString EngineSchema = "https://neurorobotics.net/engines/engine_comm_protocols.json#/engine_grpc";
};

<<<<<<< HEAD
class TestEngineGrpcClient
: public EngineGrpcClient<TestEngineGrpcClient, TestEngineGRPCConfigConst::EngineSchema, EngineGrpc::TestPayload>
=======
class TestGrpcDeviceInterface2
    : public Device<TestGrpcDeviceInterface2, "test_type2", PropNames<> >
{
    public:
		TestGrpcDeviceInterface2(DeviceIdentifier &&devID, property_template_t &&props = property_template_t())
		    : Device(std::move(devID), std::move(props))
		{}

		TestGrpcDeviceInterface2(const DeviceIdentifier &devID, const EngineGrpc::DeviceMessage &)
		    : TestGrpcDeviceInterface2(DeviceIdentifier(devID))
		{}
};

template<>
GRPCDevice DeviceSerializerMethods<GRPCDevice>::serialize<TestGrpcDeviceInterface2>(const TestGrpcDeviceInterface2 &dev)
{
	GRPCDevice data = serializeID<GRPCDevice>(dev.id());

	return data;
}

template<>
TestGrpcDeviceInterface2 DeviceSerializerMethods<GRPCDevice>::deserialize<TestGrpcDeviceInterface2>(DeviceIdentifier &&devID, deserialization_t)
{
	return TestGrpcDeviceInterface2(std::move(devID));
}


class TestEngineGrpcClient
: public EngineGrpcClient<TestEngineGrpcClient, TestEngineGRPCConfigConst::EngineSchema, TestGrpcDeviceInterface1, TestGrpcDeviceInterface2>
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
{
    public:
        TestEngineGrpcClient(nlohmann::json &config, ProcessLauncherInterface::unique_ptr &&launcher)
            : EngineGrpcClient(config, std::move(launcher))
        {}

		void initialize() override
        {
            this->sendInitCommand("test");
        }

		void shutdown() override
        {
            this->sendShutdownCommand("test");
        }
};

class TestEngineGrpcServer
<<<<<<< HEAD
    : public EngineGrpcServer<EngineGrpc::TestPayload>
=======
    : public EngineGrpcServer
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
{
    public:

        TestEngineGrpcServer(const std::string & serverAddress)
            : EngineGrpcServer(serverAddress)
        {

        }

        virtual ~TestEngineGrpcServer() override = default;

        void initialize(const nlohmann::json &data, EngineGrpcServer::lock_t &) override
        {
            specialBehaviour();

            if(data.at("throw"))
            {
                throw std::runtime_error("Init failed");
            }
        }

        void shutdown(const nlohmann::json &data) override
        {
            specialBehaviour();

            if(data.at("throw"))
            {
                throw std::runtime_error("Shutdown failed");
            }
        }

        void timeoutOnNextCommand(int sleepTimeMs = 1500)
        {
            this->_sleepTimeMs = sleepTimeMs;
        }

        SimulationTime runLoopStep(const SimulationTime timeStep) override
        {
            specialBehaviour();

            _time += timeStep;

            return _time;
        }

        void resetEngineTime()
        {
            this->_time = SimulationTime::zero();
        }

    private:

        void specialBehaviour()
        {
            if(this->_sleepTimeMs != 0)
            {
                testSleep(this->_sleepTimeMs);
                this->_sleepTimeMs = 0;
            }
        }

        SimulationTime _time = SimulationTime::zero();
        int            _sleepTimeMs = 0;
};

TEST(EngineGrpc, BASIC)
{
    // TODO This one has a linking issue, fix it!

    /*TestEngineGrpcServer               server;
    SimulationConfig::config_storage_t config;
    TestEngineGrpcClient               client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));

    ASSERT_EQ(client.getChannelStatus(), grpc_connectivity_state::GRPC_CHANNEL_IDLE);

    server.startServer();

    ASSERT_EQ(client.connect(), grpc_connectivity_state::GRPC_CHANNEL_READY);

    server.shutdownServer();*/
}

TEST(EngineGrpc, InitCommand)
{
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_grpc";

    TestEngineGrpcServer server("localhost:9004");
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));

    nlohmann::json jsonMessage;
    jsonMessage["init"]    = true;
    jsonMessage["throw"]   = false;

    // The gRPC server isn't running, so the init command should fail

    ASSERT_THROW(client.sendInitCommand(jsonMessage), std::runtime_error);

    // Start the server and send the init command. It should succeed

    server.startServer();
    // TODO Investigate why this is needed. It seems to be caused by the previous call to sendInitCommand function
    testSleep(1500);
    client.sendInitCommand(jsonMessage);

    // Force the server to return an error from the rpc
    // Check if the client receives an error response on command handling failure

    jsonMessage["throw"] = true;
    ASSERT_THROW(client.sendInitCommand(jsonMessage), std::runtime_error);
}

TEST(EngineGrpc, InitCommandTimeout)
{
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_grpc";
    config["EngineCommandTimeout"] = 0.0005;

    TestEngineGrpcServer server("localhost:9004");
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));

    nlohmann::json jsonMessage;
    jsonMessage["init"]    = true;
    jsonMessage["throw"]   = false;

    // Test init command timeout

    server.startServer();
    server.timeoutOnNextCommand();
    ASSERT_THROW(client.sendInitCommand(jsonMessage), std::runtime_error);
}

TEST(EngineGrpc, ShutdownCommand)
{
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_grpc";

    TestEngineGrpcServer server("localhost:9004");
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));

    nlohmann::json jsonMessage;
    jsonMessage["shutdown"] = true;
    jsonMessage["throw"]    = false;

    // The gRPC server isn't running, so the shutdown command should fail

    ASSERT_THROW(client.sendShutdownCommand(jsonMessage), std::runtime_error);

    // Start the server and send the shutdown command. It should succeed

    server.startServer();
    // TODO Investigate why this is needed. It seems to be caused by the previous call to sendInitCommand function
    testSleep(1500);
    ASSERT_NO_THROW(client.sendShutdownCommand(jsonMessage));

    // Force the server to return an error from the rpc
    // Check if the client receives an error response on command handling failure

    jsonMessage["throw"] = true;
    ASSERT_THROW(client.sendShutdownCommand(jsonMessage), std::runtime_error);
}

TEST(EngineGrpc, ShutdownCommandTimeout)
{
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_grpc";
    config["EngineCommandTimeout"] = 1;

    TestEngineGrpcServer server("localhost:9004");
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));

    nlohmann::json jsonMessage;
    jsonMessage["shutdown"] = true;
    jsonMessage["throw"]    = false;

    // Test shutdown command timeout

    server.startServer();
    server.timeoutOnNextCommand();
    ASSERT_THROW(client.sendShutdownCommand(jsonMessage), std::runtime_error);
}

static SimulationTime floatToSimulationTime(float time)
{
    return toSimulationTime<float, std::ratio<1>>(time);
}

TEST(EngineGrpc, RunLoopStepCommand)
{
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_grpc";

    TestEngineGrpcServer server("localhost:9004");
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));

    // The gRPC server isn't running, so the runLoopStep command should fail

    SimulationTime timeStep = floatToSimulationTime(0.1f);
    ASSERT_THROW(client.sendRunLoopStepCommand(timeStep), std::runtime_error);

    server.startServer();

    // Engine time should never be smaller than 0

    server.resetEngineTime();
    timeStep = floatToSimulationTime(-0.1f);
    ASSERT_THROW(client.sendRunLoopStepCommand(timeStep), std::runtime_error);

    // Normal loop execution, the command should return engine time

    server.resetEngineTime();
    timeStep = floatToSimulationTime(1.0f);
    ASSERT_NEAR(client.sendRunLoopStepCommand(timeStep).count(), timeStep.count(), 0.0001);

    // Try to go back in time. The client should raise an error when engine time is decreasing

    server.resetEngineTime();
    timeStep = floatToSimulationTime(2.0f);
    ASSERT_NO_THROW(client.sendRunLoopStepCommand(timeStep));
    timeStep = floatToSimulationTime(-1.0f);
    ASSERT_THROW(client.sendRunLoopStepCommand(timeStep), std::runtime_error);

    // TODO Add test for failure on server side
}

TEST(EngineGrpc, runLoopStepCommandTimeout)
{
    nlohmann::json config;
    config["EngineName"] = "engine";
    config["EngineType"] = "test_engine_grpc";
    config["EngineCommandTimeout"] = 1;

    TestEngineGrpcServer server("localhost:9004");
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));

    // Test runLoopStep command timeout

    server.startServer();
    server.timeoutOnNextCommand();
    SimulationTime timeStep = floatToSimulationTime(2.0f);
    ASSERT_THROW(client.sendRunLoopStepCommand(timeStep), std::runtime_error);
}

TEST(EngineGrpc, RegisterDevices)
{
    TestEngineGrpcServer server("localhost:9004");
<<<<<<< HEAD
    TestGrpcDeviceController *dev1 = nullptr;

    ASSERT_EQ(server.getNumRegisteredDevices(), 0);
    server.registerDevice("dev1", dev1);
=======

    TestGrpcDeviceController dev1(DeviceIdentifier("dev1", "test", "test"));

    ASSERT_EQ(server.getNumRegisteredDevices(), 0);
    server.registerDevice("dev1", &dev1);
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
    ASSERT_EQ(server.getNumRegisteredDevices(), 1);
}

TEST(EngineGrpc, SetDeviceData)
{
    const std::string deviceName = "a";
<<<<<<< HEAD
=======
    const std::string deviceType = "test_type1";
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
    const std::string engineName = "c";

    nlohmann::json config;
    config["EngineName"] = engineName;
    config["EngineType"] = "test_engine_grpc";

    TestEngineGrpcServer server("localhost:9004");
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));

    std::vector<DeviceInterface*> input_devices;

<<<<<<< HEAD
	std::shared_ptr<TestGrpcDeviceController> deviceController(new TestGrpcDeviceController()); // Server side
    server.registerDevice(deviceName, deviceController.get());

    std::shared_ptr<DataDevice<EngineGrpc::TestPayload>> dev1(new DataDevice<EngineGrpc::TestPayload>(deviceName, engineName)); // Client side
    input_devices.push_back(dev1.get());

    // The gRPC server isn't running, so the sendDevicesToEngine command should fail
    ASSERT_THROW(client.sendDevicesToEngine(input_devices), std::runtime_error);

    // Starts the Engine
    server.startServer();
    testSleep(1500);

    // After sending a device it becomes empty and sendDevicesToEngine command should fail
    ASSERT_TRUE(dev1->isEmpty());
    ASSERT_THROW(client.sendDevicesToEngine(input_devices), NRPException::exception);

    input_devices.clear();
    auto d = new EngineGrpc::TestPayload();
    d->set_integer(111);
    dev1.reset(new DataDevice<EngineGrpc::TestPayload>(deviceName, engineName, d));
    input_devices.push_back(dev1.get());

    // Normal command execution
    client.sendDevicesToEngine(input_devices);
    d = dynamic_cast<EngineGrpc::TestPayload *>(deviceController->getDeviceInformation());

	ASSERT_EQ(d->integer(),       111);

    // Test setting data on a device that wasn't registered in the engine server
    const std::string deviceName2 = "b";
    DataDevice<EngineGrpc::TestPayload> dev2(deviceName2, engineName);
    input_devices.clear();
=======
    DeviceIdentifier         devId(deviceName, engineName, deviceType);
	TestGrpcDeviceInterface1 dev1((DeviceIdentifier(devId)));             // Client side
	TestGrpcDeviceController deviceController((DeviceIdentifier(devId))); // Server side

    server.registerDevice(deviceName, &deviceController);

    input_devices.push_back(&dev1);

    // The gRPC server isn't running, so the sendDevicesToEngine command should fail

    ASSERT_THROW(client.sendDevicesToEngine(input_devices), std::runtime_error);

    // Normal command execution

    server.startServer();
    testSleep(1500);
    client.sendDevicesToEngine(input_devices);

	ASSERT_EQ(deviceController.getDeviceInformationCallback()->id().Name,       deviceName);
	ASSERT_EQ(deviceController.getDeviceInformationCallback()->id().Type,       deviceType);
	ASSERT_EQ(deviceController.getDeviceInformationCallback()->id().EngineName, engineName);

    // Test setting data on a device that wasn't registered in the engine server

    const std::string deviceName2 = "b";

    DeviceIdentifier         devId2(deviceName2, engineName, deviceType);
	TestGrpcDeviceInterface1 dev2((DeviceIdentifier(devId2)));

>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
    input_devices.push_back(&dev2);

    ASSERT_THROW(client.sendDevicesToEngine(input_devices), std::runtime_error);

    // TODO Add test for setData timeout
}

TEST(EngineGrpc, GetDeviceData)
{
    const std::string deviceName = "a";
<<<<<<< HEAD
    const std::string deviceType = "b";
=======
    const std::string deviceType = "test_type2";
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
    const std::string engineName = "c";

    nlohmann::json config;
    config["EngineName"] = engineName;
    config["EngineType"] = "test_engine_grpc";

    TestEngineGrpcServer server("localhost:9004");
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));

    // Client sends a request to the server

    std::vector<DeviceInterface*> input_devices;

    DeviceIdentifier         devId(deviceName, engineName, deviceType);
<<<<<<< HEAD
    DataDevice<EngineGrpc::TestPayload> dev1(deviceName, engineName); // Client side
    std::shared_ptr<TestGrpcDeviceController> deviceController(new TestGrpcDeviceController()); // Server side

    server.registerDevice(deviceName, deviceController.get());
=======
	TestGrpcDeviceInterface2 dev1((DeviceIdentifier(devId)));             // Client side
	TestGrpcDeviceController deviceController((DeviceIdentifier(devId))); // Server side

    server.registerDevice(deviceName, &deviceController);
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283

    input_devices.push_back(&dev1);

    EngineClientInterface::device_identifiers_set_t deviceIdentifiers;
    deviceIdentifiers.insert(devId);

    // The gRPC server isn't running, so the updateDevicesFromEngine command should fail

    ASSERT_THROW(client.updateDevicesFromEngine(deviceIdentifiers), std::runtime_error);

    server.startServer();
    testSleep(1500);
    client.sendDevicesToEngine(input_devices);

    // Return an empty device from the server
    // It should be inserted into the engines cache, but should be marked as empty

<<<<<<< HEAD
    deviceController->triggerEmptyDeviceReturn(true);
=======
    deviceController.triggerEmptyDeviceReturn(true);
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283

    auto output = client.updateDevicesFromEngine(deviceIdentifiers);

    ASSERT_EQ(output.size(), 1);
    ASSERT_EQ(output.at(0)->name(),       deviceName);
<<<<<<< HEAD
    ASSERT_EQ(output.at(0)->engineName(), engineName);
    ASSERT_EQ(output.at(0)->isEmpty(),    true);

    deviceController->triggerEmptyDeviceReturn(false);
=======
    ASSERT_EQ(output.at(0)->type(),       deviceType);
    ASSERT_EQ(output.at(0)->engineName(), engineName);
    ASSERT_EQ(output.at(0)->isEmpty(),    true);

    deviceController.triggerEmptyDeviceReturn(false);
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283

    // Normal command execution
    // Engine cache should be updated with a non-empty device

    output = client.updateDevicesFromEngine(deviceIdentifiers);

    ASSERT_EQ(output.size(), 1);
    ASSERT_EQ(output.at(0)->name(),       deviceName);
<<<<<<< HEAD
    ASSERT_EQ(output.at(0)->type(),  dev1.type());
=======
    ASSERT_EQ(output.at(0)->type(),       deviceType);
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
    ASSERT_EQ(output.at(0)->engineName(), engineName);
    ASSERT_EQ(output.at(0)->isEmpty(),    false);

    // Trigger return of an empty device again
    // Check that it doesn't overwrite the cache

<<<<<<< HEAD
    deviceController->triggerEmptyDeviceReturn(true);
=======
    deviceController.triggerEmptyDeviceReturn(true);
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283

    output = client.updateDevicesFromEngine(deviceIdentifiers);

    ASSERT_EQ(output.size(), 1);
    ASSERT_EQ(output.at(0)->isEmpty(), false);

<<<<<<< HEAD
    deviceController->triggerEmptyDeviceReturn(false);

    // Test requesting a device that wasn't registered in the engine server
=======
    deviceController.triggerEmptyDeviceReturn(false);

    // Test setting data on a device that wasn't registered in the engine server
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283

    const std::string deviceName2 = "b";

    DeviceIdentifier         devId2(deviceName2, engineName, deviceType);
<<<<<<< HEAD
=======
	TestGrpcDeviceInterface1 dev2((DeviceIdentifier(devId2)));

>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
    deviceIdentifiers.insert(devId2);

    ASSERT_THROW(client.updateDevicesFromEngine(deviceIdentifiers), std::runtime_error);

    // TODO Add test for getData timeout
}

TEST(EngineGrpc, GetDeviceData2)
{
    const std::string engineName = "c";

    const std::string deviceName1 = "a";
    const std::string deviceType1 = "test_type1";
    const std::string deviceName2 = "b";
    const std::string deviceType2 = "test_type2";

    nlohmann::json config;
    config["EngineName"] = engineName;
    config["EngineType"] = "test_engine_grpc";;

    TestEngineGrpcServer server("localhost:9004");
    TestEngineGrpcClient client(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic()));

    // Client sends a request to the server

    std::vector<DeviceInterface*> input_devices;

    DeviceIdentifier         devId1(deviceName1, engineName, deviceType1);
    DeviceIdentifier         devId2(deviceName2, engineName, deviceType2);
<<<<<<< HEAD
    DataDevice<EngineGrpc::TestPayload> dev1(deviceName1, engineName); // Client side
    DataDevice<EngineGrpc::TestPayload> dev2(deviceName2, engineName); // Client side
    std::shared_ptr<TestGrpcDeviceController> deviceController1(new TestGrpcDeviceController()); // Server side
    std::shared_ptr<TestGrpcDeviceController> deviceController2(new TestGrpcDeviceController()); // Server side

    server.registerDevice(deviceName1, deviceController1.get());
    server.registerDevice(deviceName2, deviceController2.get());
=======
	TestGrpcDeviceInterface1 dev1((DeviceIdentifier(devId1)));              // Client side
	TestGrpcDeviceInterface2 dev2((DeviceIdentifier(devId2)));              // Client side
	TestGrpcDeviceController deviceController1((DeviceIdentifier(devId1))); // Server side
	TestGrpcDeviceController deviceController2((DeviceIdentifier(devId2))); // Server side

    server.registerDevice(deviceName1, &deviceController1);
    server.registerDevice(deviceName2, &deviceController2);
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283

    input_devices.push_back(&dev1);
    input_devices.push_back(&dev2);

    server.startServer();
    client.sendDevicesToEngine(input_devices);

    EngineClientInterface::device_identifiers_set_t deviceIdentifiers;
    deviceIdentifiers.insert(devId1);
    deviceIdentifiers.insert(devId2);

    const auto output = client.updateDevicesFromEngine(deviceIdentifiers);

    ASSERT_EQ(output.size(), 2);
    ASSERT_EQ(output.at(0)->engineName(), engineName);
    ASSERT_EQ(output.at(1)->engineName(), engineName);
<<<<<<< HEAD
    ASSERT_EQ(output.at(0)->type(), dev1.type());
    ASSERT_EQ(output.at(1)->type(), dev1.type());

    if(output.at(0)->name().compare(deviceName1) == 0)
    {
        ASSERT_EQ(output.at(0)->name(), deviceName1);
        ASSERT_EQ(output.at(1)->name(), deviceName2);
=======

    if(output.at(0)->type().compare(deviceType1) == 0)
    {
        ASSERT_EQ(output.at(0)->name(), deviceName1);
        ASSERT_EQ(output.at(1)->name(), deviceName2);
        ASSERT_EQ(output.at(1)->type(), deviceType2);
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
    }
    else
    {
        ASSERT_EQ(output.at(0)->name(), deviceName2);
        ASSERT_EQ(output.at(1)->name(), deviceName1);
<<<<<<< HEAD
=======
        ASSERT_EQ(output.at(1)->type(), deviceType1);
>>>>>>> 0c552da4cd6b3368efa7cf51b04f1c46ad2e2283
    }
}

// EOF
