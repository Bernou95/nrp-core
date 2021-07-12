/* * NRP Core - Backend infrastructure to synchronize simulations
 *
 * Copyright 2020-2021 NRP Team
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

#ifndef INPUT_ROS_NODE_H
#define INPUT_ROS_NODE_H

#include <boost/python.hpp>
#include <mutex>

#include "ros/ros.h"
#include "std_msgs/String.h"

#include "nrp_general_library/device_interface/device_interface.h"
#include "nrp_general_library/engine_interfaces/engine_client_interface.h"

#include "nrp_event_loop/computational_graph/input_node.h"
#include "nrp_event_loop/python/input_edge.h"

/*!
 * \brief Input node used to connect an EngineClient with the computation graph
 *
 * It has two methods 'requestedDevices' and 'setDevices' which allows to update the node externally with the latest
 * devices
 */
class InputRosNode : public InputNode<std_msgs::String> {
public:

    /*!
     * Constructor
     */
    InputRosNode(const std::string &id) :
            InputNode(id, UpdatePolicy::LAST)
    {}

    void configure() override
    {
        ros::NodeHandle n;
        ros::Subscriber sub = n.subscribe("chatter", 1000, &InputRosNode::chatterCallback, this);
    }

protected:

    void chatterCallback(const std_msgs::String::ConstPtr& /*msg*/)
    {
        NRPLogger::debug("Received msg: ");
    }

    bool updateInputCallback(const std::string& id) override
    {
        std::lock_guard<std::mutex> lock(_devs_m);

        // move temp device to store
        if(_dev_temp) {
            _dev_store.reset(_dev_temp.get());
            _dev_temp.reset();
            return true;
        }

        return false;
    }

private:
    std::mutex _devs_m;

    std_msgs::StringConstPtr _dev_temp;
    std_msgs::StringConstPtr _dev_store;
};


class InputRosEdge : public SimpleInputEdge<std_msgs::String, InputRosNode> {

public:

    InputRosEdge(const std::string& keyword, const std::string& id) :
            SimpleInputEdge(keyword, id, id)
    {}

protected:

    InputRosNode* makeNewNode() override
    { return new InputRosNode(this->_id); }
};


#endif //INPUT_ROS_NODE_H
