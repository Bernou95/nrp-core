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

#ifndef NRP_GZ_MODEL_CONTROLLER_SYSTEM_H
#define NRP_GZ_MODEL_CONTROLLER_SYSTEM_H

#include "nrp_gz_grpc_engine/engine_server/model_datapack_controller.h"

#include <gz/sim/Link.hh>
#include <gz/sim/Model.hh>
#include <gz/sim/System.hh>

#include <memory>

namespace nrp_gz
{
    /*!
     * \brief Model system which registers a datapack for the model. The model pose and velocity can be set from NRP
     */
    class NRPModelController
            : public gz::sim::System,
              public gz::sim::ISystemConfigure,
              public gz::sim::ISystemPreUpdate,
              public gz::sim::ISystemPostUpdate
    {
        public:
            void Configure(const gz::sim::Entity &entity,
                           const std::shared_ptr<const sdf::Element> &sdf,
                           gz::sim::EntityComponentManager &ecm,
                           gz::sim::EventManager &eventMgr) override;

            void PreUpdate(const gz::sim::UpdateInfo &info, gz::sim::EntityComponentManager &ecm) override;

            void PostUpdate(const gz::sim::UpdateInfo &info, const gz::sim::EntityComponentManager &ecm) override;

        private:
            gz::sim::Model _model;

            /*!
             * \brief Canonical link of the model, used to get the model velocity
             */
            gz::sim::Link _canonicalLink;

            std::unique_ptr<ModelGrpcDataPackController> _modelInterface;

            /*!
             * \brief Whether velocity commands were set during the last step. They are removed afterwards, so that
             * they only set the model velocity once
             */
            bool _velocityCmdSet = false;
    };
}

#endif // NRP_GZ_MODEL_CONTROLLER_SYSTEM_H
