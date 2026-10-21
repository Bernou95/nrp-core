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

#ifndef GZ_CAMERA_GRPC_DATAPACK_CONTROLLER_H
#define GZ_CAMERA_GRPC_DATAPACK_CONTROLLER_H

#include "nrp_general_library/engine_interfaces/datapack_controller.h"
#include "nrp_protobuf/gazebo.pb.h"

#include <chrono>
#include <mutex>

namespace nrp_gz
{
    /*!
     * \brief Interface for cameras. Images are received asynchronously from the gz-sim camera sensor topic
     */
    class CameraGrpcDataPackController
            : public DataPackController<google::protobuf::Message>
    {
        public:
            CameraGrpcDataPackController(const std::string &devName)
                : _name(devName),
                  _data(new Gazebo::Camera())
            {}

            ~CameraGrpcDataPackController()
            {
                delete _data;
            };

            virtual void handleDataPackData(const google::protobuf::Message &) override
            {}

            virtual google::protobuf::Message *getDataPackInformation() override
            {
                std::scoped_lock lock(this->_mutex);

                if(_hasNewData) {
                    auto old = _data;
                    _data = new Gazebo::Camera();
                    _hasNewData = false;
                    return old;
                }
                else
                    return nullptr;
            }

            /*!
             * \brief Stores a new camera image
             * \param sensorTime Simulation time at which the image was taken
             */
            void updateCamData(const unsigned char *image, unsigned int width, unsigned int height, unsigned int depth,
                               std::chrono::steady_clock::duration sensorTime)
            {
                std::scoped_lock lock(this->_mutex);

                if(sensorTime > this->_lastSensorUpdateTime)
                {
                    this->_lastSensorUpdateTime = sensorTime;

                    _data->set_imageheight(height);
                    _data->set_imagewidth(width);
                    _data->set_imagedepth(depth);

                    const auto imageSize = width*height*depth;
                    _data->set_imagedata(image, imageSize);

                    _hasNewData = true;
                }
            }

            void resetTime(){
                std::scoped_lock lock(this->_mutex);
                this->_lastSensorUpdateTime = std::chrono::steady_clock::duration(-1);
                this->_hasNewData = false;
            }

        private:
            std::string _name;

            /*!
             * \brief Protects the camera data. Images arrive in a gz-transport thread
             */
            std::mutex _mutex;

            std::chrono::steady_clock::duration _lastSensorUpdateTime = std::chrono::steady_clock::duration(-1);

            Gazebo::Camera *_data;

            bool _hasNewData = false;
    };
}

#endif // GZ_CAMERA_GRPC_DATAPACK_CONTROLLER_H
