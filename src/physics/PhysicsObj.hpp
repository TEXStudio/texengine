#ifndef TEXENGINE_PHYSICSOBJ_HPP_INCLUDED
#define TEXENGINE_PHYSICSOBJ_HPP_INCLUDED

#include <btBulletDynamicsCommon.h>
#include "../asset/assetManager.hpp"
#include "../math/quaternion.hpp"
//#include "../math/vector.hpp"
#include "PhysicsStructs.hpp"
#include "../math/matrix.hpp"


using TEXEngine::Physics::PhysObjInfo;
using TEXEngine::Math::Vector3f;
using TEXEngine::Math::Vector4f;
using TEXEngine::Math::Matrix4x4f;
using TEXEngine::Math::Quaternion;


namespace TEXEngine {
	namespace Core
	{
		class PhysicsManager;
	}
	
	using Core::PhysicsManager;
	namespace Physics {

class PhysicsObj
{
	friend class PhysicsManager;

protected:

	btDiscreteDynamicsWorld* m_world;
	btRigidBody* m_rbody;

	unsigned int m_entity_id;

	int m_colID;
	int m_colWithIDs;

	bool m_active;

	Matrix4x4f m_transform_matrix;

	//Conversion from the engine matrix to an array of 16 floats
	void matrix_to_1D_array(float* outArray, const Matrix4x4f& matrix);
	void array_1D_to_matrix(Matrix4x4f& out_matrix, float* in_array);

	//this function automatically updates the 
	//transform matrix of the physics object
	virtual void update(const double dt);

	virtual void deinit(); 

	// sets collision flags when there's a listener
	void reacts();

	PhysicsObj(void);
	PhysicsObj(btDiscreteDynamicsWorld* world, 
		btRigidBody* rbody,
		unsigned int e_id,
		int colID,
		int colWithIDs);
public:
	virtual ~PhysicsObj(void);

	bool is_active();

	virtual void activate();
	virtual void deactivate();

	void no_clip();

	unsigned int entity();

	void set_transform_matrix(const Matrix4x4f& matrix);
	const Matrix4x4f& get_transform_matrix();

	const Quaternion get_quaternion();

	void set_pos(const Vector3f& pos);
	Vector3f get_pos();

	void set_rotation(const Vector4f& rot);
	void set_rotation(const Vector3f& rot);
	// returns (x,y,z,w) where (x,y,z) is an 
	// axis for rotation and w the rotation angle
	Vector4f get_rotation(); 

	void reset_velocities();
	
	void apply_velocity(const Vector3f& v);
	Vector3f get_velocity();

	void apply_force(const Vector3f& v);
	// applies force relative to orientation
	void apply_rel_force(const Vector3f& v);
	// applies torque in form of (pitch,yaw,roll)
	void apply_torque_force(const Vector3f& v);

	void apply_impulse(const Vector3f& v);
	// applies impulse relative to orientation
	void apply_rel_impulse(const Vector3f& v);
	// applies torque in form of (pitch,yaw,roll)
	void apply_torque_impulse(const Vector3f& v);

	// returns (x,y,z,w) where (x,y,z) is the
	// orientation axis and w is the rotation angle
	Vector4f get_orientation();
	Vector3f get_front_vector();

	// 0 is none, 1 is full friction
	void set_friction(float f);
	float get_friction();

	// 0 is none, 1 is full elasticity
	void set_elasticity(float e);
	float get_elasticity();

	// damping is the amount of deacceleration
	// where 0 means it doesn't stop
	// and 1 means it stops instantly
	void set_damping(float linear, float angular);
};

	} /* namespace Physics */
} /* namespace TEXEngine */

#endif /* TEXENGINE_PHYSICSOBJ_HPP_INCLUDED */
