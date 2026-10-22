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

#ifndef ROS2_PROXY_H
#define ROS2_PROXY_H

#include <rclcpp/rclcpp.hpp>

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

/*!
 * \brief Class which manages the ROS 2 node used by NRP Core to subscribe and publish to topics
 */
class NRPROS2Proxy {

public:

    // Delete move and copy operators. This ensures this class is a singleton
    NRPROS2Proxy(const NRPROS2Proxy &) = delete;
    NRPROS2Proxy(NRPROS2Proxy &&) = delete;

    NRPROS2Proxy &operator=(const NRPROS2Proxy &) = delete;
    NRPROS2Proxy &operator=(NRPROS2Proxy &&) = delete;

    ~NRPROS2Proxy();

    /*!
     * \brief Get singleton instance of NRPROS2Proxy
     */
    static NRPROS2Proxy &getInstance();

    /*!
     * \brief Whether the singleton instance has been created
     */
    static bool hasInstance()
    { return NRPROS2Proxy::_instance != nullptr; }

    /*!
     * \brief Reset singleton instance. Initializes ROS 2, if it hasn't been initialized yet, and creates a ROS 2 node
     *
     * \param nodeName Name of the ROS 2 node
     */
    static NRPROS2Proxy &resetInstance(const std::string &nodeName = "nrp_core");

    /*!
     * \brief Destroys the singleton instance and shuts down ROS 2. It is also called at exit, so that the node
     * isn't destroyed after the ROS 2 context
     */
    static void shutdown();

    /*!
     * \brief Subscribes to a topic
     *
     * \param address Address of the topic to subscribe to
     * \param callback Function to be called when a new message is received
     * \param queueSize Depth of the subscription message queue
     */
    template<class MSG_TYPE>
    void subscribe(const std::string& address, const std::function<void (const std::shared_ptr<const MSG_TYPE>&)>& callback, size_t queueSize = 10)
    { _subscriptions.push_back(_node->create_subscription<MSG_TYPE>(address, rclcpp::QoS(queueSize), callback)); }

    /*!
     * \brief Creates a publisher for a topic, if it doesn't exist yet
     *
     * Publishers are matched with subscribers asynchronously. Messages published right after creating the publisher
     * may be lost, so publishers should be created before they are used
     *
     * \param address Address of the topic to publish to
     * \param queueSize Depth of the publisher message queue
     */
    template<class MSG_TYPE>
    void advertise(const std::string& address, size_t queueSize = 10)
    {
        if(!_publishers.count(address))
            _publishers.emplace(address, _node->create_publisher<MSG_TYPE>(address, rclcpp::QoS(queueSize)));
    }

    /*!
     * \brief Publishes a message to a topic. A publisher is created on the first call for a given topic, if it
     * hasn't been created with advertise() before
     *
     * \param address Address of the topic to publish to
     * \param msg Message to be published
     * \param queueSize Depth of the publisher message queue
     */
    template<class MSG_TYPE>
    void publish(const std::string& address, const MSG_TYPE& msg, size_t queueSize = 10)
    {
        this->advertise<MSG_TYPE>(address, queueSize);
        std::static_pointer_cast<rclcpp::Publisher<MSG_TYPE>>(_publishers.at(address))->publish(msg);
    }

    /*!
     * \brief Processes all the ROS 2 callbacks which are ready, e.g. received messages. As ros::spinOnce, all the
     * messages waiting in the subscription queues are processed (rclcpp::spin_some only takes one per subscription)
     */
    void spinOnce()
    { rclcpp::spin_all(_node, std::chrono::nanoseconds(0)); }

    /*!
     * \brief ROS 2 node used by the proxy
     */
    rclcpp::Node::SharedPtr node()
    { return _node; }

private:

    /*!
     * \brief ROS 2 node
     */
    rclcpp::Node::SharedPtr _node;

    /*!
     * \brief Publishers, one per topic address
     */
    std::map<std::string, rclcpp::PublisherBase::SharedPtr> _publishers;

    /*!
     * \brief Subscriptions
     */
    std::vector<rclcpp::SubscriptionBase::SharedPtr> _subscriptions;

    /*!
     * \brief Constructor
     */
    NRPROS2Proxy(const std::string &nodeName);

    static std::unique_ptr<NRPROS2Proxy> _instance;
};

#endif //ROS2_PROXY_H
