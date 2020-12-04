#ifndef PHYSICS_LINK_H
#define PHYSICS_LINK_H

#include "nrp_general_library/device_interface/device.h"
#include "nrp_general_library/utils/serializers/json_property_serializer.h"

class PhysicsLink;

struct PhysicsLinkConst
{
		using vec3_t = std::array<float, 3>;
		using quat_t = std::array<float, 4>;

		static constexpr FixedString Position = "pos";
		static constexpr vec3_t DefPosition = {0,0,0};

		static constexpr FixedString Rotation = "rot";
		static constexpr quat_t DefRotation = {0,0,0,0};

		static constexpr FixedString LinearVelocity = "lin_vel";
		static constexpr vec3_t DefLinearVelocity = {0,0,0};

		static constexpr FixedString AngularVelocity = "ang_vel";
		static constexpr vec3_t DefAngularVelocity = {0,0,0};

		using JPropNames = PropNames<Position, Rotation, LinearVelocity, AngularVelocity>;
};

class PhysicsLink
        : public PhysicsLinkConst,
          public Device<PhysicsLink, "PhysicsLink", PhysicsLinkConst::JPropNames, std::array<float, 3>, std::array<float, 4>, std::array<float, 3>, std::array<float, 3> >
{
	public:
		PhysicsLink(DeviceIdentifier &&devID, property_template_t &&props = property_template_t(DefPosition, DefRotation, DefLinearVelocity, DefAngularVelocity))
		    : Device(std::move(devID), std::move(props))
		{}

		template<class DESERIALIZE_T>
		static auto deserializeProperties(DESERIALIZE_T &&data)
		{	return Device::deserializeProperties(std::forward<DESERIALIZE_T>(data), DefPosition, DefRotation, DefLinearVelocity, DefAngularVelocity);	}

		const vec3_t &position() const;
		void setPosition(const vec3_t &position);

		const quat_t &rotation() const;
		void setRotation(const quat_t &rotation);

		const vec3_t &linVel() const;
		void setLinVel(const vec3_t &linVel);

		const vec3_t &angVel() const;
		void setAngVel(const vec3_t &angVel);
};

#endif // JSON_PHYSICS_LINK_H
