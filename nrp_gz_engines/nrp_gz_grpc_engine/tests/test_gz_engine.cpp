/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2023 NRP Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project has received funding from the European Union’s Horizon 2020
 * Framework Programme for Research and Innovation under the Specific Grant
 * Agreement No. 945539 (Human Brain Project SGA3).
 */

#include <gtest/gtest.h>

#include "nrp_gz_grpc_engine/config/gz_grpc_config.h"
#include "nrp_gz_grpc_engine/nrp_client/gz_engine_grpc_nrp_client.h"
#include "nrp_general_library/process_launchers/process_launcher_basic.h"

#include "tests/test_env_cmake.h"

static constexpr int MAX_DATA_ACQUISITION_TRIALS = 10;

namespace
{
    nlohmann::json createConfig(const std::string &worldFile)
    {
        nlohmann::json config;
        config["EngineName"] = "engine";
        config["EngineType"] = "gz_grpc";
        config["GazeboWorldFile"] = worldFile;
        config["GazeboRNGSeed"] = 12345;

        return config;
    }

    PtrTemplates<GzEngineGrpcNRPClient>::shared_ptr launchEngine(nlohmann::json &config)
    {
        GzEngineGrpcLauncher launcher;
        return std::dynamic_pointer_cast<GzEngineGrpcNRPClient>(
                launcher.launchEngine(config, ProcessLauncherInterface::unique_ptr(new ProcessLauncherBasic())));
    }

    template<class DATA_TYPE>
    DATA_TYPE getData(PtrTemplates<GzEngineGrpcNRPClient>::shared_ptr &engine, const std::string &datapackName)
    {
        const auto datapacks = engine->getDataPacksFromEngine({DataPackIdentifier(datapackName, engine->engineName(), "irrelevant_type")});
        EXPECT_EQ(datapacks.size(), 1);

        const auto *dataPack = dynamic_cast<const DataPack<DATA_TYPE> *>(datapacks.at(0).get());
        EXPECT_NE(dataPack, nullptr);

        return dataPack->getData();
    }
}

TEST(TestGzGrpcEngine, InvalidWorld)
{
    auto config = createConfig("non_existent_world.sdf");

    auto engine = launchEngine(config);
    ASSERT_NE(engine, nullptr);

    ASSERT_ANY_THROW(engine->initialize());
}

TEST(TestGzGrpcEngine, Stepping)
{
    auto config = createConfig(TEST_EMPTY_WORLD_FILE);

    auto engine = launchEngine(config);
    ASSERT_NE(engine, nullptr);

    ASSERT_NO_THROW(engine->initialize());

    // A loop step runs timeStep / max_step_size iterations
    ASSERT_NO_THROW(engine->runLoopStepAsync(toSimulationTime<int, std::milli>(100)));
    ASSERT_NO_THROW(engine->runLoopStepAsyncGet(toSimulationTimeFromSeconds(5.0)));
    ASSERT_EQ(engine->getEngineTime(), (toSimulationTime<int, std::milli>(100)));

    ASSERT_NO_THROW(engine->runLoopStepAsync(toSimulationTime<int, std::milli>(50)));
    ASSERT_NO_THROW(engine->runLoopStepAsyncGet(toSimulationTimeFromSeconds(5.0)));
    ASSERT_EQ(engine->getEngineTime(), (toSimulationTime<int, std::milli>(150)));

    ASSERT_NO_THROW(engine->reset());
    ASSERT_EQ(engine->getEngineTime(), SimulationTime::zero());

    // The engine simulation time has been reset as well
    ASSERT_NO_THROW(engine->runLoopStepAsync(toSimulationTime<int, std::milli>(100)));
    ASSERT_NO_THROW(engine->runLoopStepAsyncGet(toSimulationTimeFromSeconds(5.0)));
    ASSERT_EQ(engine->getEngineTime(), (toSimulationTime<int, std::milli>(100)));

    ASSERT_NO_THROW(engine->shutdown());
}

TEST(TestGzGrpcEngine, JointLinkAndModelSystems)
{
    auto config = createConfig(TEST_PENDULUM_WORLD_FILE);

    auto engine = launchEngine(config);
    ASSERT_NE(engine, nullptr);

    ASSERT_NO_THROW(engine->initialize());

    // Joint datapack
    ASSERT_EQ(getData<Gazebo::Joint>(engine, "pendulum::hinge").position(), 0);

    // Link and model datapacks
    const auto link = getData<Gazebo::Link>(engine, "pendulum::arm");
    ASSERT_EQ(link.position_size(), 3);
    ASSERT_NEAR(link.position(2), 0.75, 1e-4);

    const auto model = getData<Gazebo::Model>(engine, "pendulum");
    ASSERT_EQ(model.position_size(), 3);
    ASSERT_NEAR(model.position(2), 1.0, 1e-4);

    // Set a velocity target. The velocity PID of the hinge joint should make it rotate
    auto newJointDev = new Gazebo::Joint();
    newJointDev->set_effort(NAN);
    newJointDev->set_velocity(1.0f);
    newJointDev->set_position(NAN);

    datapacks_set_t outputDataPacks;
    outputDataPacks.insert(std::shared_ptr<DataPackInterface>(new DataPack<Gazebo::Joint>("pendulum::hinge",
                                                                                           engine->engineName(),
                                                                                           newJointDev)));
    ASSERT_NO_THROW(engine->sendDataPacksToEngine(outputDataPacks));

    ASSERT_NO_THROW(engine->runLoopStepAsync(toSimulationTime<int, std::milli>(500)));
    ASSERT_NO_THROW(engine->runLoopStepAsyncGet(toSimulationTimeFromSeconds(5.0)));

    const auto joint = getData<Gazebo::Joint>(engine, "pendulum::hinge");
    ASSERT_GT(joint.position(), 0.1);
    ASSERT_NEAR(joint.velocity(), 1.0, 0.2);

    // Set the joint position
    newJointDev = new Gazebo::Joint();
    newJointDev->set_effort(NAN);
    newJointDev->set_velocity(NAN);
    newJointDev->set_position(-1.0f);

    outputDataPacks.clear();
    outputDataPacks.insert(std::shared_ptr<DataPackInterface>(new DataPack<Gazebo::Joint>("free_hinge",
                                                                                           engine->engineName(),
                                                                                           newJointDev)));
    // Joint datapacks are scoped with the model name
    ASSERT_ANY_THROW(engine->sendDataPacksToEngine(outputDataPacks));

    newJointDev = new Gazebo::Joint();
    newJointDev->set_effort(NAN);
    newJointDev->set_velocity(NAN);
    newJointDev->set_position(-1.0f);

    outputDataPacks.clear();
    outputDataPacks.insert(std::shared_ptr<DataPackInterface>(new DataPack<Gazebo::Joint>("pendulum::free_hinge",
                                                                                           engine->engineName(),
                                                                                           newJointDev)));
    ASSERT_NO_THROW(engine->sendDataPacksToEngine(outputDataPacks));

    ASSERT_NO_THROW(engine->runLoopStepAsync(toSimulationTime<int, std::milli>(1)));
    ASSERT_NO_THROW(engine->runLoopStepAsyncGet(toSimulationTimeFromSeconds(5.0)));
    ASSERT_NEAR(getData<Gazebo::Joint>(engine, "pendulum::free_hinge").position(), -1.0, 1e-2);

    // Reset restores the initial state
    ASSERT_NO_THROW(engine->reset());
    ASSERT_NEAR(getData<Gazebo::Joint>(engine, "pendulum::hinge").position(), 0, 1e-6);
    ASSERT_NEAR(getData<Gazebo::Joint>(engine, "pendulum::free_hinge").position(), 0, 1e-6);

    ASSERT_NO_THROW(engine->shutdown());
}

TEST(TestGzGrpcEngine, AdditionalModels)
{
    auto config = createConfig(TEST_EMPTY_WORLD_FILE);
    config["GazeboSDFModels"] = {{{"Name", "box1"}, {"File", TEST_BOX_MODEL_FILE}, {"InitPose", "0 0 2 0 0 0"}},
                                 {{"Name", "box2"}, {"File", TEST_BOX_MODEL_FILE}, {"InitPose", "1 0 3 0 0 0"}}};

    auto engine = launchEngine(config);
    ASSERT_NE(engine, nullptr);

    ASSERT_NO_THROW(engine->initialize());

    ASSERT_NEAR(getData<Gazebo::Model>(engine, "box1").position(2), 2.0, 1e-4);
    ASSERT_NEAR(getData<Gazebo::Model>(engine, "box2").position(0), 1.0, 1e-4);

    // Boxes fall due to gravity
    ASSERT_NO_THROW(engine->runLoopStepAsync(toSimulationTime<int, std::milli>(200)));
    ASSERT_NO_THROW(engine->runLoopStepAsyncGet(toSimulationTimeFromSeconds(5.0)));
    ASSERT_LT(getData<Gazebo::Model>(engine, "box1").position(2), 2.0);
    ASSERT_LT(getData<Gazebo::Model>(engine, "box1").linearvelocity(2), -1.0);

    // Set model pose
    auto newModelDev = new Gazebo::Model();
    newModelDev->add_position(5);
    newModelDev->add_position(5);
    newModelDev->add_position(5);

    datapacks_set_t outputDataPacks;
    outputDataPacks.insert(std::shared_ptr<DataPackInterface>(new DataPack<Gazebo::Model>("box2",
                                                                                           engine->engineName(),
                                                                                           newModelDev)));
    ASSERT_NO_THROW(engine->sendDataPacksToEngine(outputDataPacks));

    ASSERT_NO_THROW(engine->runLoopStepAsync(toSimulationTime<int, std::milli>(1)));
    ASSERT_NO_THROW(engine->runLoopStepAsyncGet(toSimulationTimeFromSeconds(5.0)));
    ASSERT_NEAR(getData<Gazebo::Model>(engine, "box2").position(0), 5.0, 1e-2);

    ASSERT_NO_THROW(engine->shutdown());
}

TEST(TestGzGrpcEngine, CameraSystem)
{
    auto config = createConfig(TEST_CAMERA_WORLD_FILE);

    auto engine = launchEngine(config);
    ASSERT_NE(engine, nullptr);

    ASSERT_NO_THROW(engine->initialize());

    // Images are rendered asynchronously. Step the simulation until the first image has been received
    datapacks_vector_t datapacks;
    int trial = 0;
    do
    {
        ASSERT_NO_THROW(engine->runLoopStepAsync(toSimulationTime<int, std::milli>(100)));
        ASSERT_NO_THROW(engine->runLoopStepAsyncGet(toSimulationTimeFromSeconds(5.0)));
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        datapacks = engine->getDataPacksFromEngine({DataPackIdentifier("camera::link::camera", engine->engineName(), "irrelevant_type")});
        ASSERT_EQ(datapacks.size(), 1);
    }
    while(datapacks.begin()->get()->isEmpty() && trial++ < MAX_DATA_ACQUISITION_TRIALS);

    ASSERT_LE(trial, MAX_DATA_ACQUISITION_TRIALS);

    const DataPack<Gazebo::Camera> * camDat = dynamic_cast<const DataPack<Gazebo::Camera>*>(datapacks.begin()->get());
    ASSERT_NE(camDat, nullptr);
    ASSERT_EQ(camDat->getData().imageheight(), 240);
    ASSERT_EQ(camDat->getData().imagewidth(),  320);
    ASSERT_EQ(camDat->getData().imagedepth(),  3);
    ASSERT_EQ(camDat->getData().imagedata().size(), 320*240*3);

    ASSERT_NO_THROW(engine->shutdown());
}
