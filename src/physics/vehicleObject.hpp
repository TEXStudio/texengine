#ifndef TEXENGINE_VEHICLEOBJECT_HPP_INCLUDED
#define TEXENGINE_VEHICLEOBJECT_HPP_INCLUDED

/***Note that in its current state the vehicle
only has a bounding box collision shape and it only has four
wheels...for now******/
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Vehicle/btRaycastVehicle.h>
#include "PhysicsStructs.hpp"
#include "PhysicsObj.hpp"

class btVehicleTuning;
struct btVehicleRaycaster;
class btCollisionShape;

namespace TEXEngine
{
	namespace Core
	{
		class PhysicsManager;
	}
	using Core::PhysicsManager;

	namespace Physics 
	{

		class VehicleObject : public PhysicsObj
		{
			friend class PhysicsManager;
		public:
			void set_vehicle_properties(const VehicleProperties& properties);
			void activate();
			void deactivate();
			//Directly apply turning angle
			void turn_left(const float fvalue);
			void turn_right(const float fvalue);
			//Turning angle calculated internally using
			//the steering increment defined in vehicle properties struct
			void turn_left_increment();
			void turn_right_increment();
			void not_turning(); //Must be callled when steering is not being applied
			//Directly apply engine force
			void accelerate(const float fvalue); //Applying a negative value reverses
			//Engine force calculated internally using
			//the engine force increment defined in vehicle properties struct
			void accelerate_increment();
			void reverse_increment();
			void not_accelerating(); //must be called when engine force is not being applied
			void brake();
			void brake(const float fvalue);
			void not_braking(); //Must be called each frame when braking is not being applied

			float get_steering();
			float get_steer_inc();
		protected:
			VehicleObject();
			VehicleObject(btDiscreteDynamicsWorld* world, unsigned int e_id, int colID, int colWithIDs);
			~VehicleObject();
			
			void deinit(); // TODO: delete all internal shapes
			void init_vehicle_physics(const Vector3f& box_size, const PhysObjInfo& info,
									  const VehicleProperties& properties);

			virtual void update(const double dt);

		private:
			void shutdown();
			void init_wheels();
			void init_engine();
			void init_vehicle();
			void update_wheels();

			VehicleProperties m_vehicle_properties;
			std::vector<btCollisionShape*> m_pListOfCollisionShapes;
			btVector3 m_box_size;

			btRaycastVehicle::btVehicleTuning m_vehicle_tuning;
			btVehicleRaycaster*	m_pvehicleRayCaster;
			btRaycastVehicle*	m_pvehicle;

			/***Used for defining the vehicles coordinate system***/
			int m_iright_index;
			int m_iup_index;
			int m_iforward_index;
			/******************************************************/

			btVector3 m_vWheelDirCS0; //The direction inwhich the wheels roll
			btVector3 m_vWheelAxelCS;
			//
			//Driving
			float m_fEngineForce; //The amount of driving force to apply to vehicle(driving wheels)
			float m_fBrakingForce; //The amount of braking force to be applied to selected wheels 
			float m_fVehicleSteering; //Amount of steering to be applied to front wheels
		};
	}
}

#endif //TEXENGINE_VEHICLEOBJECT_HPP_INCLUDED