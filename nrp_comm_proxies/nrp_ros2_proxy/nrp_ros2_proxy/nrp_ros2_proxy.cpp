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

#include "nrp_ros2_proxy/nrp_ros2_proxy.h"

#include "nrp_general_library/utils/nrp_exceptions.h"

std::unique_ptr<NRPROS2Proxy> NRPROS2Proxy::_instance = nullptr;

NRPROS2Proxy::NRPROS2Proxy(const std::string &nodeName)
{
    // NRP Core handles signals itself, ROS 2 must not install its own handlers
    if(!rclcpp::ok())
        rclcpp::init(0, nullptr, rclcpp::InitOptions(), rclcpp::SignalHandlerOptions::None);

    _node = std::make_shared<rclcpp::Node>(nodeName);
}

NRPROS2Proxy::~NRPROS2Proxy()
{
    _subscriptions.clear();
    _publishers.clear();
    _node.reset();
}

NRPROS2Proxy &NRPROS2Proxy::getInstance()
{
    if(NRPROS2Proxy::_instance == nullptr)
        throw NRPException::logCreate("NRPROS2Proxy has not been initialized. Add the \"ROSNode\" parameter to the experiment configuration");

    return *(NRPROS2Proxy::_instance);
}

NRPROS2Proxy &NRPROS2Proxy::resetInstance(const std::string &nodeName)
{
    NRPROS2Proxy::_instance.reset(new NRPROS2Proxy(nodeName));

    // Functions registered with atexit run before the destruction of the static objects created before, such as
    // the ROS 2 context. Make sure the node is always destroyed first
    static bool registeredAtExit = false;
    if(!registeredAtExit)
    {
        std::atexit(&NRPROS2Proxy::shutdown);
        registeredAtExit = true;
    }

    return NRPROS2Proxy::getInstance();
}

void NRPROS2Proxy::shutdown()
{
    NRPROS2Proxy::_instance.reset();

    if(rclcpp::ok())
        rclcpp::shutdown();
}
