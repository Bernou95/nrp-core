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

#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <chrono>
#include <thread>

#include "nrp_event_loop/computational_graph/computational_graph_manager.h"
#include "nrp_event_loop/computational_graph/input_node.h"
#include "nrp_event_loop/computational_graph/output_node.h"

#include "nrp_general_library/utils/python_interpreter_state.h"

#include <nlohmann/json.hpp>

/*!
 * \brief Manages simulation loop. Runs physics and brain interface, and synchronizes them via Transfer Functions
 */
class EventLoop
{

    // TODO: implement runasync and runonce

	public:

        virtual ~EventLoop();

		EventLoop() = delete;

        /*!
         * \brief Constructor
         */
		EventLoop(const nlohmann::json &config, std::chrono::milliseconds timestep, bool ownGIL = true);

        /*!
         * \brief Run loop in a thread
         */
        void runLoopAsync();

		/*!
		 * \brief Shutdown loop
		 */
		void shutdownLoop();

        ComputationalNode* getInputNode(const std::string& id);

        ComputationalNode* getOutputNode(const std::string& id);

	private:

        /*!
         * \brief Run loop
         */
        void runLoop();

        std::unique_ptr<std::thread> _loop_thread;

        std::chrono::milliseconds _timestep;

        std::atomic<bool> _doRun;

        bool _ownGIL;
        PyGILState_STATE _pyGILState;
};


#endif // EVENT_LOOP_H
