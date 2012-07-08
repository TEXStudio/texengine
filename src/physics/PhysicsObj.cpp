#include <new>
#include "../math/precision.h"
#include "PhysicsObj.hpp"

using TEXEngine::Math::scalar_t;

namespace TEXEngine {
	namespace Physics {

PhysicsObj::PhysicsObj(void) :	
	m_active(false),
	m_world(0), 
	m_rbody(0),
	m_entity_id(0),
	m_colID(0),
	m_colWithIDs(0)
{
}

PhysicsObj::PhysicsObj(
	btDiscreteDynamicsWorld* world, 
	btRigidBody* rbody,
	unsigned int e_id,
	int colID,
	int colWithIDs) 
		:	
	m_active(false),
	m_world(world), 
	m_rbody(rbody),
	m_entity_id(e_id),
	m_colID(colID),
	m_colWithIDs(colWithIDs)
{
	if(rbody)
		rbody->setUserPointer(&m_entity_id);
}

PhysicsObj::~PhysicsObj(void)
{
}

bool PhysicsObj::is_active()
{
	return m_active;
}

void PhysicsObj::update(const double dt)
{
	/*
	if(m_rbody)
	{
		//update transform matrix
		float temp[16];
		btTransform bt_transform;
		m_rbody->getMotionState()->getWorldTransform(bt_transform);

		//Apply bullet tranformation to engine matrix
		bt_transform.getOpenGLMatrix(temp);
		array_1D_to_matrix(m_transform_matrix, temp);
	}*/
}

void PhysicsObj::activate()
{
	if(!m_active)
	{
		if(m_colID <= 0)
			m_world->addRigidBody(m_rbody);
		else
			m_world->addRigidBody(m_rbody,m_colID,m_colWithIDs);

		m_active = true;
	}
}

void PhysicsObj::deactivate()
{	
	if(m_active)
	{
		m_world->removeRigidBody(m_rbody);
		m_active = false;
	}
}

void PhysicsObj::no_clip()
{
	m_rbody->setCollisionFlags(m_rbody->getCollisionFlags() ^ btCollisionObject::CF_NO_CONTACT_RESPONSE);
	m_rbody->activate();
}

void PhysicsObj::deinit()
{

}

void PhysicsObj::reacts()
{
	// necessary for the callback to be called
	m_rbody->setCollisionFlags(m_rbody->getCollisionFlags() |
		btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
}

unsigned int PhysicsObj::entity()
{
	return m_entity_id;
}

void PhysicsObj::set_pos(const Vector3f& pos) 
{ 
	//_rbody->setActivationState(ACTIVE_TAG);

	btTransform trans;
	//m_rbody->getMotionState()->getWorldTransform(trans);
	trans = m_rbody->getWorldTransform();
	m_rbody->translate(btVector3((btScalar)pos.x - trans.getOrigin().getX(), (btScalar)pos.y - trans.getOrigin().getY(), (btScalar)pos.z - trans.getOrigin().getZ()));

	m_rbody->activate();
}

Vector3f PhysicsObj::get_pos() 
{ 
	btTransform trans; 
	//m_rbody->getMotionState()->getWorldTransform(trans); 
	trans = m_rbody->getWorldTransform();
	return Vector3f(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ()); 
}

void PhysicsObj::reset_velocities()
{
	m_rbody->setAngularVelocity(btVector3(0,0,0));
	m_rbody->setLinearVelocity(btVector3(0,0,0));
}

void PhysicsObj::set_rotation(const Vector4f& rot) // this isn't properly implemented
{/*
	btTransform trans;
	m_rbody->getMotionState()->getWorldTransform(trans);
	trans.setRotation(btQuaternion(btVector3((btScalar)rot.x,(btScalar)rot.y,(btScalar)rot.z),(btScalar)rot.w));
	m_rbody->getMotionState()->setWorldTransform(trans);

	m_rbody->activate(true);*/

	//rigidBody->setCenterOfMassTransform(tr);

}

void PhysicsObj::set_rotation(const Vector3f& rot) // this isn't properly implemented
{/*
	btTransform trans;
	m_rbody->getMotionState()->getWorldTransform(trans);
	trans.setRotation(btQuaternion((btScalar)rot.x,(btScalar)rot.y,(btScalar)rot.z));
	m_rbody->getMotionState()->setWorldTransform(trans);

	m_rbody->activate(true);*/
}

Vector4f PhysicsObj::get_rotation()
{
	btTransform trans;
	m_rbody->getMotionState()->getWorldTransform(trans);
	btQuaternion qt = trans.getRotation();
	btVector3 axis = qt.getAxis();
	return Vector4f(axis.getX(),axis.getY(),axis.getZ(),qt.getAngle());
}



void PhysicsObj::apply_velocity(const Vector3f& v)
{
	m_rbody->setLinearVelocity(btVector3((btScalar)v.x,(btScalar)v.y,(btScalar)v.z));

	m_rbody->activate();
}

Vector3f PhysicsObj::get_velocity()
{
	btVector3 v = m_rbody->getLinearVelocity();
	return Vector3f(v.getX(),v.getY(),v.getZ());
}

void PhysicsObj::apply_force(const Vector3f& v)
{
	m_rbody->applyCentralForce(btVector3((btScalar)v.x,(btScalar)v.y,(btScalar)v.z));

	m_rbody->activate();
}

void PhysicsObj::apply_rel_force(const Vector3f& v)
{
	btTransform trans;
	m_rbody->getMotionState()->getWorldTransform(trans);
	btVector3 correctedForce = trans * btVector3((btScalar)v.x,(btScalar)v.y,(btScalar)v.z);
	correctedForce -= trans.getOrigin();
	m_rbody->applyCentralForce(correctedForce);

	m_rbody->activate();
}

void PhysicsObj::apply_torque_force(const Vector3f& v)
{
	btTransform trans;
	m_rbody->getMotionState()->getWorldTransform(trans);
	btVector3 correctedForce = trans * btVector3((btScalar)v.x,(btScalar)v.y,(btScalar)v.z);
	correctedForce -= trans.getOrigin();

	m_rbody->applyTorque(correctedForce);

	m_rbody->activate();
}

void PhysicsObj::apply_impulse(const Vector3f& v)
{
	m_rbody->applyCentralImpulse(btVector3((btScalar)v.x,(btScalar)v.y,(btScalar)v.z));

	m_rbody->activate();
}

void PhysicsObj::apply_rel_impulse(const Vector3f& v)
{
	btTransform trans;
	m_rbody->getMotionState()->getWorldTransform(trans);
	btVector3 correctedForce = trans * btVector3((btScalar)v.x,(btScalar)v.y,(btScalar)v.z);
	correctedForce -= trans.getOrigin();
	m_rbody->applyCentralImpulse(correctedForce);

	m_rbody->activate();
}

void PhysicsObj::apply_torque_impulse(const Vector3f& v)
{
	btTransform trans;
	m_rbody->getMotionState()->getWorldTransform(trans);
	btVector3 correctedForce = trans * btVector3((btScalar)v.x,(btScalar)v.y,(btScalar)v.z);
	correctedForce -= trans.getOrigin();

	m_rbody->applyTorqueImpulse(correctedForce);

	m_rbody->activate();
}

Vector4f PhysicsObj::get_orientation()
{
	btQuaternion qt = m_rbody->getOrientation();

	btVector3 axis = qt.getAxis();
	return Vector4f(axis.getX(),axis.getY(),axis.getZ(),qt.getAngle());
}

Vector3f PhysicsObj::get_front_vector()
{
	btQuaternion q = m_rbody->getOrientation();
	Vector3f v = Vector3f(1.0f,0.0f,0.0f);

	return v + cross( Vector3f(q.x(),q.y(),q.z()) , cross( Vector3f(q.x(),q.y(),q.z()) , v )+( v * q.w() ) )*2.0f;
}

const Quaternion PhysicsObj::get_quaternion()
{
	btQuaternion qt = m_rbody->getOrientation();
	return Quaternion(qt.x(),qt.y(),qt.z(),qt.w());
}

void PhysicsObj::set_friction(float f)
{
	m_rbody->setFriction(f);
}

float PhysicsObj::get_friction()
{
	return m_rbody->getFriction();
}


void PhysicsObj::set_elasticity(float e)
{
	m_rbody->setRestitution(e);
}

float PhysicsObj::get_elasticity()
{
	return m_rbody->getRestitution();
}

void PhysicsObj::set_damping(float linear, float angular)
{
	m_rbody->setDamping(linear,angular);
}

void PhysicsObj::set_transform_matrix(const Matrix4x4f& matrix)
{
	m_transform_matrix = matrix;

	float temp[16];
	Matrix4x4f mat = matrix;
	matrix_to_1D_array(temp, mat);

	btTransform transform;
	transform.setFromOpenGLMatrix(temp);
	//apply transformation
	m_rbody->setWorldTransform(transform);  

	m_rbody->activate(true);
}
const Matrix4x4f& PhysicsObj::get_transform_matrix()
{
	//update transform matrix
	float temp[16];
	btTransform bt_transform;
	//m_rbody->getMotionState()->getWorldTransform(bt_transform);
	bt_transform = m_rbody->getWorldTransform();

	//Apply bullet tranformation to engine matrix
	bt_transform.getOpenGLMatrix(temp);
	array_1D_to_matrix(m_transform_matrix, temp);

	return m_transform_matrix;
}

//Conversion from the engine matrix to an array of 16 floats
void PhysicsObj::matrix_to_1D_array(float* outArray, const Matrix4x4f& matrix)
{
	Matrix4x4f mat = matrix;
	for(int i = 0; i < 16; i++)
	{
		outArray[i] = (float)*mat[i];
	}
}

void PhysicsObj::array_1D_to_matrix(Matrix4x4f& out_matrix, float* in_array)
{
	for(int i = 0; i < 16; i++)
	{
		*out_matrix[i] = (scalar_t)in_array[i];
	}
}

	} /* namespace Physics */
} /* namespace TEXEngine */