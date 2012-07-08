#ifndef TEXENGINE_PHYSICSSTRUCTS_H_INCLUDED
#define TEXENGINE_PHYSICSSTRUCTS_H_INCLUDED

#include <vector>
#include "../math/vector.hpp"
#include "../math/matrix.hpp"

using TEXEngine::Math::Vector3f;
using TEXEngine::Math::Matrix4x4f;

namespace TEXEngine {
	namespace Physics {


struct PhysObjInfo
{
	PhysObjInfo() : colId(-1), colWithIds(NULL), mass(1) 
	{
		m_initial_transform_matrix.load_identity();
	};

	int colId; 
	std::vector<int>* colWithIds;

	float mass;
	Matrix4x4f m_initial_transform_matrix;

	//Default values
	void apply_default()
	{
		m_initial_transform_matrix.load_identity();
		colId = -1;
		mass = 1;
		colWithIds = NULL;
	}
};

struct VehicleProperties
{
	float m_fWheelHeight;
	float m_fEngineForceIncr; //How to increment the engine force when accelerating
	float m_fMaxEngineForce; //Engine force limition...determines max speed of vehicle
	float m_fMaxBrakingForce; //The braking force applied to vehicle when braking
	float m_fSteerIncr; //The amount to increment the steering angle
	float m_fSteerClamp; //limit the amount of turning angle...determines how well/quickly vehicle turns
	float m_fWheelRadius;
	float m_fWheelWidth;
	float m_fWheelFriction; //friction between wheels and ground
	float m_fSuspStiffness; //determines how 'bouncy or rigid' the vehicle will be
	float m_fSuspDamping;
	float m_fSuspCompression;
	float m_fRollInfluence;
	float m_fSuspRestLength;
	float m_fWheelPosOffset_x;
	float m_fWheelPosOffset_y;
	float m_fWheelPosOffset_z;


	VehicleProperties() :
		m_fWheelHeight(1.2f),
		m_fEngineForceIncr(10.0f),
		m_fMaxEngineForce(1000.0f),
		m_fMaxBrakingForce(100.0f),
		m_fSteerIncr(0.04f),
		m_fSteerClamp(0.8f),
		m_fWheelRadius(0.5f),
		m_fWheelWidth(0.4f),
		m_fWheelFriction(1000.0f),
		m_fSuspStiffness(20.0f),
		m_fSuspDamping(2.3f),
		m_fSuspCompression(4.4f),
		m_fRollInfluence(0.1f),
		m_fSuspRestLength(0.6f),
		m_fWheelPosOffset_x(1.0f),
		m_fWheelPosOffset_y(1.0f),
		m_fWheelPosOffset_z(1.0f)
	{
	}
		
	//Default values
	void apply_default()
	{
		m_fWheelHeight = 1.2f;
		m_fEngineForceIncr = 10.0f;
		m_fMaxEngineForce = 1000.0f;
		m_fMaxBrakingForce = 100.0f;
		m_fSteerIncr = 0.04f;
		m_fSteerClamp = 0.8f;
		m_fWheelRadius = 0.5f;
		m_fWheelWidth = 0.4f;
		m_fWheelFriction = 1000.0f;
		m_fSuspStiffness = 20.0f;
		m_fSuspDamping = 2.3f;
		m_fSuspCompression = 4.4f;
		m_fRollInfluence = 0.1f;
		m_fSuspRestLength = 0.6f;
		m_fWheelPosOffset_x = 1.0f;
		m_fWheelPosOffset_y = 1.0f;
		m_fWheelPosOffset_z = 1.0f;
	}
};

	} /* namespace Physics */
} /* namespace TEXEngine */

#endif /* TEXENGINE_PHYSICSSTRUCTS_HPP_INCLUDED */