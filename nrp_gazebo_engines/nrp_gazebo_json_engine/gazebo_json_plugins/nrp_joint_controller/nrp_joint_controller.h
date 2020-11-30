#ifndef NRP_JOINT_CONTROLLER_H
#define NRP_JOINT_CONTROLLER_H

#include "nrp_gazebo_json_engine/config/cmake_constants.h"
#include "nrp_gazebo_devices/physics_joint.h"
#include "nrp_json_engine_protocol/engine_server/engine_json_device_controller.h"
#include "nrp_general_library/utils/nrp_exceptions.h"

#include <gazebo/gazebo.hh>
#include <gazebo/physics/JointController.hh>
#include <gazebo/physics/Joint.hh>

namespace gazebo
{
	/*!
	 * \brief Interface for a single joint
	 */
	class JointDeviceController
	        : public EngineJSONDeviceController<PhysicsJoint>
	{
			using fcn_ptr_t = void(physics::JointPtr, double, int);

		public:
			JointDeviceController(const physics::JointPtr &joint, const physics::JointControllerPtr &jointController, const std::string &jointName);

			virtual void handleDeviceDataCallback(PhysicsJoint &&data) override;
			virtual const PhysicsJoint *getDeviceInformationCallback() override;

		private:
			/*!
			 * \brief Pointer to joint
			 */
			physics::JointPtr _joint;

			/*!
			 * \brief Pointer to joint controller of the joint's model
			 */
			physics::JointControllerPtr _jointController = nullptr;

			/*!
			 * \brief Data of link
			 */
			PhysicsJoint _jointData;

			template<JointDeviceController::fcn_ptr_t T>
			void setDeviceData(double input);
	};

	class NRPJointController
	        : public gazebo::ModelPlugin
	{
			struct PIDConfig
			        : public common::PID
			{
				enum PID_TYPE { VELOCITY, POSITION };

				PID_TYPE Type = POSITION;

				PIDConfig(common::PID _pid, PID_TYPE _type);

				static PID_TYPE convertStringToType(std::string type);
			};

		public:
			virtual ~NRPJointController() override;

			virtual void Load(physics::ModelPtr model, sdf::ElementPtr sdf);

		private:

			/*!
			 * \brief List containing all joint interfaces. TODO: Change to shared_ptr to prevent segfault errors when this plugin is destroyed
			 */
			std::list<JointDeviceController> _jointDeviceControllers;

			template<class T>
			static T getOptionalValue(const sdf::ElementPtr &pidConfig, const std::string &key, T defaultValue);
	};

	GZ_REGISTER_MODEL_PLUGIN(NRPJointController)

	template<class T>
	T NRPJointController::getOptionalValue(const sdf::ElementPtr &pidConfig, const std::string &key, T defaultValue)
	{
		try
		{
			return pidConfig->Get<T>(key);
		}
		catch(std::exception &e)
		{
			NRPException::logOnce(e);
		}

		return defaultValue;
	}
}

#endif
