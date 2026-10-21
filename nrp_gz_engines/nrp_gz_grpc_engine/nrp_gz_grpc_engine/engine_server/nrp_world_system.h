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

#ifndef NRP_GZ_WORLD_SYSTEM_H
#define NRP_GZ_WORLD_SYSTEM_H

#include "nrp_general_library/utils/time_utils.h"

#include <gz/sim/System.hh>

namespace nrp_gz
{
    /*!
     * \brief gz-sim system added by GzGrpcEngine to every simulated world. Keeps track of the simulation time
     */
    class NRPWorldSystem
            : public gz::sim::System,
              public gz::sim::ISystemPostUpdate
    {
        public:
            void PostUpdate(const gz::sim::UpdateInfo &info, const gz::sim::EntityComponentManager &ecm) override;

            /*!
             * \brief Simulation time after the last executed iteration
             */
            SimulationTime simTime() const
            {   return this->_simTime;  }

        private:
            SimulationTime _simTime = SimulationTime::zero();
    };
}

#endif // NRP_GZ_WORLD_SYSTEM_H
