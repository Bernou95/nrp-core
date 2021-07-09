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
#include <functional>

#include <gtest/gtest.h>

#include "nrp_event_loop/computational_graph/computational_node.h"
#include "nrp_event_loop/computational_graph/computational_graph_manager.h"
#include "nrp_event_loop/computational_graph/input_port.h"
#include "nrp_event_loop/computational_graph/output_port.h"
#include "nrp_event_loop/computational_graph/functional_node.h"
#include "nrp_event_loop/computational_graph/functional_node_factory.h"

class TestNode : public ComputationalNode {
public:

    TestNode(const std::string &id, NodeType type) :
            ComputationalNode(id, type)
    {}

    void configure() override
    {}

    void compute() override
    {}
};

struct TestMsg {
    int value;
};

template <>
struct dataConverter<int, TestMsg> {
    static void convert(const int* d1, TestMsg& d2)
    { d2.value = *d1; }
};


TEST(ComputationalGraph, PORT_PUBLISH_NO_CONVERSION)
{
    TestNode n1("input", ComputationalNode::Input);
    TestNode n2("output", ComputationalNode::Output);

    int msg_send = 1;
    const int* msg_got = nullptr;
    std::function<void(const int*)> f = [&](const int* a) { msg_got = a; };

    OutputPort<int> o_p("output_port", &n1);
    InputPort<int, int> i_p("input_port", &n2, f);
    i_p.subscribeTo(&o_p);

    o_p.publish(&msg_send);
    ASSERT_EQ(msg_got, &msg_send);
}

TEST(ComputationalGraph, PORT_PUBLISH_CONVERSION)
{
    TestNode n1("input", ComputationalNode::Input);
    TestNode n2("output", ComputationalNode::Output);

    int msg_send = 1;
    const TestMsg* msg_got = nullptr;
    std::function<void(const TestMsg*)> f = [&](const TestMsg* a) { msg_got = a; };

    OutputPort<int> o_p("output_port", &n1);
    InputPort<int, TestMsg> i_p("input_port", &n2, f);
    i_p.subscribeTo(&o_p);

    o_p.publish(&msg_send);
    ASSERT_EQ(msg_got->value, msg_send);
}

TEST(ComputationalGraph, PORT_SUBSCRIPTORS)
{
    TestNode n1("input", ComputationalNode::Input);
    TestNode n2("output", ComputationalNode::Output);

    std::function<void(const int*)> f = [&](const int*) { };

    OutputPort<int> o_p("output_port", &n1);
    InputPort<int, int> i_p("input_port", &n2, f, 1);

    ASSERT_EQ(o_p.subscriptionsSize(), 0);
    ASSERT_EQ(i_p.subscriptionsSize(), 0);

    i_p.subscribeTo(&o_p);

    ASSERT_EQ(o_p.subscriptionsSize(), 1);
    ASSERT_EQ(i_p.subscriptionsSize(), 1);

    ASSERT_THROW(i_p.subscribeTo(&o_p), NRPException);
}

TEST(ComputationalGraph, FUNCTIONAL_NODE)
{
    TestNode n1("input", ComputationalNode::Input);
    TestNode n2("output", ComputationalNode::Output);

    int msg_send = 1;
    const int* msg_got = nullptr;
    std::function<void(const int*)> f = [&](const int* a) { msg_got = a; };

    OutputPort<int> o_p("output_port", &n1);
    InputPort<int, int> i_p("input_port", &n2, f);

    std::function<void(const int*, int&)> f1 = [](const int*i1, int&o1) {
        if(i1 != nullptr)
            o1 = *i1;
    };

    auto n = FunctionalNodeFactory::create<1, 1, const int*, int&>("f_node", f1);
    auto i_pn = n.registerInput<0, int, int>("input");
    auto o_pn = n.registerOutput<0, int>("output");

    i_p.subscribeTo(o_pn);
    i_pn->subscribeTo(&o_p);

    o_p.publish(&msg_send);
    n.compute();
    ASSERT_EQ(*msg_got, msg_send);
}

// EOF
