#ifndef TEXENGINE_PHYSICSMANAGER_HPP_INCLUDED
#define TEXENGINE_PHYSICSMANAGER_HPP_INCLUDED

#include <vector>
#include <hash_map>
#include <btBulletDynamicsCommon.h>
#include "PhysicsStructs.hpp"
#include "PhysicsObj.hpp"
#include "vehicleObject.hpp"
#include "CollisionListener.hpp"
#include "../asset/assetManager.hpp"
#include "physicsDebugRenderer.hpp"

using TEXEngine::Physics::CollisionListener;
using TEXEngine::Physics::PhysicsObj;
using TEXEngine::Physics::PhysObjInfo;
using TEXEngine::Math::Vector3f;
using TEXEngine::Physics::PhysicsDebugRenderer;
using TEXEngine::Asset::Mesh;
using TEXEngine::Physics::VehicleObject;
using TEXEngine::Physics::VehicleProperties;


namespace TEXEngine {
	namespace Core {

class PhysicsManager
{
private:
	static PhysicsManager m_physicsmanager;

	std::hash_map<unsigned int,CollisionListener*> m_listeners_map;
	std::vector<btCollisionShape*> m_colshapes_list;
	std::vector<btRigidBody*> m_rbodies_list;
	std::vector<PhysicsObj*> m_pobjs_list;
	std::vector<btTriangleMesh*> m_trimesh_list;

	btBroadphaseInterface* m_broadphase;
	btDefaultCollisionConfiguration* m_collisionConfiguration;
	btCollisionDispatcher* m_dispatcher;
	btSequentialImpulseConstraintSolver* m_solver;
	btDiscreteDynamicsWorld* m_dynamicsWorld;

	PhysicsObj* create_object(const PhysObjInfo& info, btCollisionShape* shape, unsigned int entity_id);

	~PhysicsManager(void);
	PhysicsManager(PhysicsManager const&);
	void operator=(PhysicsManager const&);

	void deinit();

	//Phyiscs debug renderer
	PhysicsDebugRenderer* m_pDebug_renderer;
	bool m_bEnable_debug_render;

	

	//Creating objects
	btTriangleMesh* create_btTriMesh_from_mesh(Mesh* pMesh, const Vector3f& scale);

protected:
	PhysicsManager();

public:
	//Conversion from the engine matrix to an array of 16 floats
	void matrix_to_1D_array(float* outArray, const Matrix4x4f& matrix);
	void array_1D_to_matrix(Matrix4x4f& out_matrix, float* in_array);

	static PhysicsManager& get();

	bool custom_collision_callback(btManifoldPoint& cp,
		const btCollisionObject* colObj0, int partId0, int index0, 
		const btCollisionObject* colObj1, int partId1, int index1);

	void init();
	void update(const double dt);
	

	void set_gravity(const Vector3f& v);
	Vector3f get_gravity();

	void register_listener(unsigned int eID, CollisionListener* cl);

	

	//debug rendering
	void set_enable_debug_render(const bool enable);
	bool get_enable_debug_render();
	//This function must always be called right after rendering the entire 3d scene
	//if the enable variable is true the debug rendering will begin otherwise nothing
	//will happpen
	void debug_render();

	//Physics object creation functions
	PhysicsObj* create_sphere(const PhysObjInfo& info, 
							unsigned int entity_id,
							float radius);

	// sizes specifies the x, y and z dimensions
	PhysicsObj* create_box(const PhysObjInfo& info, 
							unsigned int entity_id, 
							const Vector3f& sizes);

	PhysicsObj* create_cylinder(const PhysObjInfo& info, 
								unsigned int entity_id, 
								float radius, 
								float height);

	// offset is the offset of the plane from the origin along the normal
	PhysicsObj* create_plane(const PhysObjInfo& info, 
							unsigned int entity_id, 
							const Vector3f& normal, 
							float offset);
	
	PhysicsObj* create_static_object_from_mesh(Mesh* pMesh, const Vector3f& scale,
											   unsigned int entity_id, 
											   const PhysObjInfo& info);

	VehicleObject* create_vehicle(const Vector3f& box_size, /**size of bounding box**/
								  unsigned int entity_id,  
								  const PhysObjInfo& info,
								  const VehicleProperties& properties);

};

	} /* namespace Core */
} /* namespace TEXEngine */

#endif /* TEXENGINE_PHYSICSMANAGER_HPP_INCLUDED */
