#include "vehicleObject.hpp"


namespace TEXEngine
{
	namespace Physics 
	{
		VehicleObject::VehicleObject() : PhysicsObj(),
		m_pvehicleRayCaster(0),
		m_pvehicle(0),
		m_iright_index(0),
		m_iup_index(0),
		m_iforward_index(0),
		m_fEngineForce(0),
		m_fBrakingForce(0),
		m_fVehicleSteering(0)
		{

		}
		VehicleObject::VehicleObject(btDiscreteDynamicsWorld* world, unsigned int e_id, int colID, int colWithIDs) :
			PhysicsObj(world, NULL, e_id, colID, colWithIDs),
			m_pvehicleRayCaster(0),
			m_pvehicle(0),
			m_iright_index(0),
			m_iup_index(0),
			m_iforward_index(0),
			m_fEngineForce(0),
			m_fBrakingForce(0),
			m_fVehicleSteering(0)
		{}

		VehicleObject::~VehicleObject()
		{
			//shutdown();
		}

		void VehicleObject::activate()
		{
			if(!m_active)
			{
				m_world->addVehicle(m_pvehicle);
			}
			PhysicsObj::activate();
		}

		void VehicleObject::deactivate()
		{
			
			if(m_active)
			{
				m_world->removeVehicle(m_pvehicle);
			}
			PhysicsObj::deactivate();
		}

		void VehicleObject::deinit()
		{
			shutdown();
		}

		void VehicleObject::shutdown()
		{
			if(m_pvehicleRayCaster)
			{
				delete m_pvehicleRayCaster;
				m_pvehicleRayCaster = 0;
			}
			if(m_pvehicle)
			{
				delete m_pvehicle;
				m_pvehicle = 0;
			}
	
			//Delete collision shapes
			for(int j=0; j < (int)m_pListOfCollisionShapes.size(); j++)
			{
				btCollisionShape* pShape = m_pListOfCollisionShapes[j];
				delete pShape;
			}

			//Clear list
			m_pListOfCollisionShapes.clear();

			if(m_rbody)
			{
				delete m_rbody->getMotionState();
				delete m_rbody;
				m_rbody = 0;
			}
		}

		void VehicleObject::set_vehicle_properties(const VehicleProperties& properties)
		{
			m_vehicle_properties = properties;
		}
		//Directly apply turning angle
		void VehicleObject::turn_left(const float fvalue)
		{
			m_fVehicleSteering += fvalue;
			if(m_fVehicleSteering > m_vehicle_properties.m_fSteerClamp)
			{
				m_fVehicleSteering = m_vehicle_properties.m_fSteerClamp;
			}
		}
		void VehicleObject::turn_right(const float fvalue)
		{
			m_fVehicleSteering -= fvalue;
			if(m_fVehicleSteering < -m_vehicle_properties.m_fSteerClamp)
			{
				m_fVehicleSteering = -m_vehicle_properties.m_fSteerClamp;
			}
		}
		//Turning angle calculated internally using
		//the steering increment defined in vehicle properties struct
		void VehicleObject::turn_left_increment()
		{
			m_fVehicleSteering += m_vehicle_properties.m_fSteerIncr;
			if(m_fVehicleSteering > m_vehicle_properties.m_fSteerClamp)
			{
				m_fVehicleSteering = m_vehicle_properties.m_fSteerClamp;
			}
		}
		void VehicleObject::turn_right_increment()
		{
			m_fVehicleSteering -= m_vehicle_properties.m_fSteerIncr;
			if(m_fVehicleSteering < -m_vehicle_properties.m_fSteerClamp)
			{
				m_fVehicleSteering = -m_vehicle_properties.m_fSteerClamp;
			}
		}
		void VehicleObject::not_turning() //Must be callled when steering is not being applied
		{
			
			if(m_fVehicleSteering > 0)
			{
				m_fVehicleSteering -= m_vehicle_properties.m_fSteerIncr;
				if(m_fVehicleSteering < 0)
					m_fVehicleSteering = 0;
			}
			else if(m_fVehicleSteering < 0)
			{
				m_fVehicleSteering += m_vehicle_properties.m_fSteerIncr;
				if(m_fVehicleSteering > 0)
					m_fVehicleSteering = 0;
			}
		}
		//Directly apply engine force
		void VehicleObject::accelerate(const float fvalue) //Applying a negative value reverses
		{
			m_fEngineForce = fvalue;
			if(m_fEngineForce > m_vehicle_properties.m_fMaxEngineForce)
			{
				m_fEngineForce = m_vehicle_properties.m_fMaxEngineForce;
			}
		}
		//Engine force calculated internally using
		//the engine force increment defined in vehicle properties struct
		void VehicleObject::accelerate_increment()
		{
			m_fEngineForce += m_vehicle_properties.m_fEngineForceIncr;
			if(m_fEngineForce > m_vehicle_properties.m_fMaxEngineForce)
			{
				m_fEngineForce = m_vehicle_properties.m_fMaxEngineForce;
			}
			else if(m_fEngineForce < -0.5f*m_vehicle_properties.m_fMaxEngineForce)
			{
				m_fEngineForce = -0.5f*m_vehicle_properties.m_fMaxEngineForce;
			}
		}
		void VehicleObject::reverse_increment()
		{
			m_fEngineForce -= 0.5f*m_vehicle_properties.m_fEngineForceIncr;
			if(m_fEngineForce < -0.5f*m_vehicle_properties.m_fMaxEngineForce)
			{
				m_fEngineForce = -0.5f*m_vehicle_properties.m_fMaxEngineForce;
			}
		}
		void VehicleObject::not_accelerating() //must be called when engine force is not being applied
		{
			m_fEngineForce = 0.0f;
		}
		void VehicleObject::brake()
		{
			m_fBrakingForce = m_vehicle_properties.m_fMaxBrakingForce;
		}
		void VehicleObject::brake(const float fvalue)
		{
			m_fBrakingForce += fvalue;
			if(m_fBrakingForce > m_vehicle_properties.m_fMaxBrakingForce)
			{
				m_fBrakingForce = m_vehicle_properties.m_fMaxBrakingForce;
			}
		}
		void VehicleObject::not_braking() //Must be called each frame when braking is not being applied
		{
			m_fBrakingForce = 0.0f;
		}

		void VehicleObject::init_vehicle_physics(const Vector3f& box_size, const PhysObjInfo& info,
											     const VehicleProperties& properties)
		{

			m_iright_index = 0;
			m_iup_index = 1;
			m_iforward_index = 2;

			set_vehicle_properties(properties);

			m_box_size = btVector3((btScalar)box_size.x, (btScalar)box_size.y, (btScalar)box_size.z);

			//Physics stuff
			btCollisionShape* pChassisShape = new btBoxShape(m_box_size);
			m_pListOfCollisionShapes.push_back(pChassisShape);

			btCompoundShape* pCompoundShape = new btCompoundShape();
			m_pListOfCollisionShapes.push_back(pCompoundShape);

			btTransform localTrans;
			localTrans.setIdentity();
			localTrans.setOrigin(btVector3(0, 1, 0));

			pCompoundShape->addChildShape(localTrans, pChassisShape);

			//Setup chassis
			btTransform chassisTrans;
			chassisTrans.setIdentity();
			chassisTrans.setOrigin(btVector3(0, 0, 0));

			btVector3 localInertia(0, 0, 0);
			pCompoundShape->calculateLocalInertia(info.mass, localInertia);
			btDefaultMotionState* vehicleMotionState = new btDefaultMotionState(chassisTrans);
			btRigidBody::btRigidBodyConstructionInfo cInfo(info.mass, vehicleMotionState, pCompoundShape, localInertia);

			if(m_rbody)
			{
				delete m_rbody;
				m_rbody = 0;
			}

			m_rbody = new btRigidBody(cInfo);
			m_rbody->setContactProcessingThreshold(1e18f);
			m_rbody->setUserPointer(&m_entity_id);

			//initialise other parts of the vehicle
			init_engine();
			init_vehicle();
			init_wheels();

		}


		void VehicleObject::update(const double dt)
		{

			if(m_pvehicle)
			{
				//update wheels
				update_wheels();

				//update transform matrix
				float temp[16];
				btTransform bt_transform;
				bt_transform = m_pvehicle->getChassisWorldTransform();

				//Apply bullet tranformation to engine matrix
				bt_transform.getOpenGLMatrix(temp);
				//temp[13] += 1.0f;
				array_1D_to_matrix(m_transform_matrix, temp);
			}
		}

		float VehicleObject::get_steer_inc()
		{
			return m_vehicle_properties.m_fSteerIncr;
		}

		float VehicleObject::get_steering()
		{
			return m_fVehicleSteering;
		}

		void VehicleObject::init_wheels()
		{

			m_vWheelDirCS0 = btVector3(0, -1, 0);
			m_vWheelAxelCS = btVector3(0, 0, -1);

			//Wheel connection
			float fWheelHeight = m_vehicle_properties.m_fWheelHeight;
			float fWheelRadius = m_vehicle_properties.m_fWheelRadius;
			float fSuspRestLength = m_vehicle_properties.m_fSuspRestLength;
			float fWheelWidth = m_vehicle_properties.m_fWheelWidth;
			float halfExtx = m_vehicle_properties.m_fWheelPosOffset_x;
			float halfExty = m_vehicle_properties.m_fWheelPosOffset_y;
			float halfExtz = m_vehicle_properties.m_fWheelPosOffset_z;
			bool m_bIsFrontWheel = true;

			btVector3 connectionPointCS0(m_box_size.x()*halfExtx-fWheelRadius, fWheelHeight, halfExtz-(0.3f*fWheelWidth));


			//Add front wheels
			m_pvehicle->addWheel(connectionPointCS0, m_vWheelDirCS0, m_vWheelAxelCS, fSuspRestLength,
								 fWheelRadius, m_vehicle_tuning, m_bIsFrontWheel);

			connectionPointCS0 = btVector3(m_box_size.x()*halfExtx-fWheelRadius, fWheelHeight,-halfExtz+(0.3f*fWheelWidth) );
											m_pvehicle->addWheel(connectionPointCS0, m_vWheelDirCS0, m_vWheelAxelCS, fSuspRestLength, 
											fWheelRadius, m_vehicle_tuning, m_bIsFrontWheel);

			//Add rear wheels
			m_bIsFrontWheel = false;
			connectionPointCS0 = btVector3(-m_box_size.x()*halfExtx+fWheelRadius, fWheelHeight,-halfExtz+(0.3f*fWheelWidth) );
											m_pvehicle->addWheel(connectionPointCS0, m_vWheelDirCS0, m_vWheelAxelCS, fSuspRestLength, fWheelRadius,
											m_vehicle_tuning, m_bIsFrontWheel);

			connectionPointCS0 = btVector3(-m_box_size.x()*halfExtx+fWheelRadius, fWheelHeight,halfExtz-(0.3f*fWheelWidth) );
										   m_pvehicle->addWheel(connectionPointCS0, m_vWheelDirCS0, m_vWheelAxelCS, fSuspRestLength, fWheelRadius,
										   m_vehicle_tuning, m_bIsFrontWheel);

			//
			for(int i =0; i < m_pvehicle->getNumWheels(); i++)
			{
				btWheelInfo& wheel = m_pvehicle->getWheelInfo(i);
				wheel.m_suspensionStiffness = m_vehicle_properties.m_fSuspStiffness;
				wheel.m_wheelsDampingRelaxation = m_vehicle_properties.m_fSuspDamping;
				wheel.m_wheelsDampingCompression = m_vehicle_properties.m_fSuspCompression;
				wheel.m_frictionSlip = m_vehicle_properties.m_fWheelFriction;
				wheel.m_rollInfluence = m_vehicle_properties.m_fRollInfluence;
			}
		}
		void VehicleObject::init_engine()
		{
			m_fEngineForce = 0.0f;
			m_fBrakingForce = 0.0f;
			m_fVehicleSteering = 0.0f;
		}
		void VehicleObject::init_vehicle()
		{
			m_pvehicleRayCaster = new btDefaultVehicleRaycaster(m_world);
			m_pvehicle = new btRaycastVehicle(m_vehicle_tuning, m_rbody, m_pvehicleRayCaster);

			//Never deactive vehicle
			m_rbody->setActivationState(DISABLE_DEACTIVATION);


			//Setup coordinate system for vehicle
			m_pvehicle->setCoordinateSystem(m_iright_index, m_iup_index, m_iforward_index);
		}
		void VehicleObject::update_wheels()
		{
			//float transformMat[16];
			for (int i=0;i<m_pvehicle->getNumWheels();i++)
			{
				//synchronize the wheels with the (interpolated) chassis worldtransform
				m_pvehicle->updateWheelTransform(i,true);

				//m_pvehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(transformMat);
			}
			//Applying forces
			//Rear wheel drive thus apply engine force and braking to rear wheels
			m_pvehicle->applyEngineForce(-m_fEngineForce, 2);
			m_pvehicle->setBrake(m_fBrakingForce, 2);

			m_pvehicle->applyEngineForce(-m_fEngineForce, 3);
			m_pvehicle->setBrake(m_fBrakingForce, 3);

			//Apply steering to front wheels
			m_pvehicle->setSteeringValue(m_fVehicleSteering, 0);
			m_pvehicle->setBrake(m_fBrakingForce, 0);
			m_pvehicle->setSteeringValue(m_fVehicleSteering, 1);
			m_pvehicle->setBrake(m_fBrakingForce, 1);
		}
	}
}