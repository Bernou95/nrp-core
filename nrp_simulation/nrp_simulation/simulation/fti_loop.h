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

#ifndef FTI_LOOP_H
#define FTI_LOOP_H

#include "nrp_general_library/engine_interfaces/engine_client_interface.h"

#include "nrp_general_library/utils/json_schema_utils.h"

#include "nrp_simulation/datapack_handle/datapack_handle.h"


/*!
 * \brief Defines order of DataPack operations within a single simulation step
 *
 * Two workflows are supported:
 * - ORIGINAL:
 *   The order of operations is the following:
 *   1. Wait for end of execution of simulation steps for all engines
 *   2. Retrieve DataPacks from all engines
 *   3. Execute functions (PFs, TFs)
 *   4. Send DataPacks to Engines
 *   5. Execute simulations steps for all engines selected for this iteration in asyncronous mode
 * - RL:
 *   The order of operations is the following:
 *   1. Send DataPacks to Engines
 *   2. Execute simulations steps for all engines selected for this iteration in asyncronous mode
 *   3. Wait for end of execution of simulation steps for all engines
 *   4. Retrieve DataPacks from all engines
 *   5. Execute functions (PFs, TFs)
 */
enum class SimulationLoopType
{
    ORIGINAL,
    RL,
};

/*!
 * \brief Manages simulation loop. Runs physics and brain interface, and synchronizes them via Transfer Functions
 */
class FTILoop
        : public PtrTemplates<FTILoop>
{
    public:

        using engine_queue_t = std::multimap<SimulationTime, EngineClientInterfaceSharedPtr>;

        FTILoop() = default;
        FTILoop(jsonSharedPtr config, DataPackProcessor::engine_interfaces_t engines, SimulationDataManager * simulationDataManager, SimulationTime timestep);

        /*!
         * \brief Initialize engines before running loop
         */
        void initLoop();

        /*!
         * \brief Reset engines of the loop
         */
        void resetLoop();

        /*!
         * \brief Shutdown engines
         */
        void shutdownLoop();

        /*!
         * \brief Blocks until all running Engines finishe their execution
         */
        void waitForEngines();

        /*!
         * \brief Runs the requested number of simulation steps
         *
         * Variables and data structures:
         * t  - current simulation time
         * dt - simulation timestep
         * Q  - priority queue which holds pointers to all engines of the simulation
         *      sorted by their current engine time t_e, that is the simulation time at which their last
         *      simulation step has ended. Simulation of each engine is advanced by its timestep dt_e.
         *
         * Initial conditions:
         * - For each engine in Q, t_e = 0
         *
         * Algorithm:
         * 1. Request a simulation step with timestep dt.
         * 2. Find all engines in the priority queue Q, for which t_e <= t, and put them in another queue W.
         * 3. Run the loop for all engines in W. The run_loop step will be executed in parallel for all engines,
         *    even if there are small differences in their current engine time t_e (within the current simulation timestep).
         * 4. For all engines in W, update their engine time t_e += dt_e, and push them back into the queue Q.
         * 5. Update current simulation time t += dt.
         *
         * Remarks:
         * - Engine time is always tracked using the "perfect" (predicted) time instead of the real engine time.
         *   This allows for small fluctuations in the actual engine time returned by the engines, which can be a result
         *   of floating point arithmetics or internal timesteps of the simulators, without disturbing the algorithm.
         *   This is especially important for the point 3. of the algorithm, where any discrepancies in engine time could
         *   change the number of engines chosen for the current loop step.
         * - The actual order of operations on DataPacks and calls to engine's API depends on the type of loop that is
         *   specified in the configuration file
         *
         * \param numIterations Number of iterations to run
         */
        void runLoop(int numIterations);

        /*!
         * \brief Get Simulation Time (in seconds)
         * \return Returns time passed in simulation (in seconds)
         */
        inline SimulationTime getSimTime() const
        {   return this->_simTime;  }

    private:

        /*!
         * \brief Returns a list of engines that should run in the next iteration
         *
         * The method scans the priority queue for engines with engine time smaller or equal
         * to the current simulation time. These engines are extracted from the queue, and returned.
         *
         * \param timeUntil Simulation time that will be compared with the engine time of all engines
         */
        engine_queue_t getEnginesForCurrentStep(SimulationTime timeUntil);

        /*!
         * \brief Initializes engine priority queue with proper values
         */
        void initEngineQueue(const DataPackProcessor::engine_interfaces_t & engines);

        /*!
         * \brief Executes a single simulation step with the original order of operations
         *
         * The order of operations is the following:
         * 1. Wait for end of execution of simulation steps for all engines
         * 2. Retrieve DataPacks from all engines
         * 3. Execute functions (PFs, TFs)
         * 4. Send DataPacks to Engines
         * 5. Execute simulations steps for all engines selected for this iteration in asyncronous mode
         *
         * \param engines A set of engines selected for this iteration
         */
        void runLoopOriginal(engine_queue_t & engines);

        /*!
         * \brief Executes a single simulation step with order of operations suitable for RL applications
         *
         * The order of operations is the following:
         * 1. Send DataPacks to Engines
         * 2. Execute simulations steps for all engines selected for this iteration in asyncronous mode
         * 3. Wait for end of execution of simulation steps for all engines
         * 4. Retrieve DataPacks from all engines
         * 5. Execute functions (PFs, TFs)
         *
         * \param engines A set of engines selected for this iteration
         */
        void runLoopRL(engine_queue_t & engines);

        /*!
         * \brief Configuration of simulation
         */
        jsonSharedPtr _config;

        /*!
         * \brief Engines
         */
        DataPackProcessor::engine_interfaces_t _engines;

        /*!
         * \brief Engine priority queue, sorted by engine time
         */
        engine_queue_t _engineQueue;

        /*!
         * \brief Used to handle datapack operations in engines
         */
        std::unique_ptr<DataPackProcessor> _devHandler;

        /*!
         * \brief Current simulation time
         */
        SimulationTime _simTime     = SimulationTime::zero();

        /*!
         * \brief Current simulation iteration number
         */
        unsigned long _simIteration = 0;

        /*!
         * \brief Simulation timestep
         */
        SimulationTime _simTimestep = SimulationTime::zero();

        /*!
         * \brief Type of simulation loop that determines order of DataPack operations
         */
        SimulationLoopType _loopType;

        friend class FTILoopTest_InitTFManager_Test;
};

using FTILoopSharedPtr = FTILoop::shared_ptr;
using FTILoopConstSharedPtr = FTILoop::const_shared_ptr;

#endif // FTI_LOOP_H
