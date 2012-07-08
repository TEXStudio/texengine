#include "../math/precision.h"
#include "PhysicsManager.hpp"
#include "../log/logmanager.hpp"

using TEXEngine::Math::scalar_t;

static bool CustomMaterialCombinerCallback(btManifoldPoint& cp,	
	const btCollisionObject* colObj0,int partId0,int index0,
	const btCollisionObject* colObj1,int partId1,int index1)
{
	return TEXEngine::Core::PhysicsManager::get().custom_collision_callback(cp,	
		colObj0,partId0,index0,
		colObj1,partId1,index1);
}

extern ContactAddedCallback gContactAddedCallback;

namespace TEXEngine {
	namespace Core {

PhysicsManager PhysicsManager::m_physicsmanager;

PhysicsManager::PhysicsManager() :
m_pDebug_renderer(0),
m_bEnable_debug_render(false)
{
}

PhysicsManager::~PhysicsManager(void) 
{ 
	deinit(); 
}

PhysicsManager& PhysicsManager::get() 
{ 
	return  m_physicsmanager;
}

void PhysicsManager::update(const double dt)
{
	m_dynamicsWorld->stepSimulation((btScalar) dt);

	//Update the physics objects
	for(int i = 0; i < (int)m_pobjs_list.size(); ++i)
	{
		m_pobjs_list[i]->update(dt);
	}
}

void PhysicsManager::register_listener(unsigned int eID, CollisionListener* cl)
{
	m_listeners_map[eID] = cl;
	Entity* ent = AssetManager::get().get_entity(eID);
	if (ent)
	{
		PhysicsObj* po = ent->physics_obj();
		if(po)
			po->reacts();
	}
}

bool PhysicsManager::custom_collision_callback(btManifoldPoint& cp,	
	const btCollisionObject* colObj0,int partId0,int index0,
	const btCollisionObject* colObj1,int partId1,int index1)
{
	if (!colObj0 || !colObj1) 
		return false;

	unsigned int e1 = *(unsigned int*)(colObj0->getUserPointer());
	unsigned int e2 = *(unsigned int*)(colObj1->getUserPointer());

	//TEXEngine::Core::LogManager::get().log_message("in custom_collision_callback");

	if(e1 && e2)
	{
		//TEXEngine::Core::LogManager::get().log_message("   got e1=%i and e2=%i; ", e1, e2);


		std::hash_map<unsigned int,CollisionListener*>::iterator cl1 = m_listeners_map.find(e1);
		if(cl1 != m_listeners_map.end() && cl1->second){
			cl1->second->react_to(e2);
			//TEXEngine::Core::LogManager::get().log_message("   calling react_to(e2)");
		}

		std::hash_map<unsigned int,CollisionListener*>::iterator cl2 = m_listeners_map.find(e2);
		if(cl2 != m_listeners_map.end() && cl2->second){
			cl2->second->react_to(e1);
			//TEXEngine::Core::LogManager::get().log_message("   calling react_to(e1)");
		}
	}

	return false;
}


void PhysicsManager::init()
{
	gContactAddedCallback = CustomMaterialCombinerCallback;

	// Build the broadphase
	btVector3 vWorldAaabbMin(-10000, -10000, -10000);
	btVector3 vWorldAaabbMax(10000, 10000, 10000);
    m_broadphase =  new btAxisSweep3(vWorldAaabbMin, vWorldAaabbMax, 1024);
 
    // Set up the collision configuration and dispatcher
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

 
    // The actual physics solver
    m_solver = new btSequentialImpulseConstraintSolver;
 
    // The world. default -10j m/s^2 gravity
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0,-10,0));

	//debug renderer
	if(!m_pDebug_renderer)
	{
		m_pDebug_renderer =  new PhysicsDebugRenderer();
		//set wireframe debug rendering
		m_pDebug_renderer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

		//Apply debug render to bullet dynamics world
		m_dynamicsWorld->setDebugDrawer(m_pDebug_renderer);
	}

}

void PhysicsManager::deinit()
{
	// removes rigid bodies from world
	for(int i = 0; i < (int)m_pobjs_list.size(); ++i)
	{
		PhysicsObj* pobj = m_pobjs_list[i];
		if(pobj)
			pobj->deactivate();
	}

	// delete rigid bodies
	for(int i = 0; i < (int)m_rbodies_list.size(); ++i)
	{
		btRigidBody* rbody = m_rbodies_list[i];
		
		if(rbody)
		{
			delete rbody->getMotionState();
			delete rbody;
		}
	}
	m_rbodies_list.clear();

	// delete collision shapes
	for(int i = 0; i < (int)m_colshapes_list.size(); ++i)
	{
		btCollisionShape* colshape = m_colshapes_list[i];

		if(colshape)
		{
			delete colshape;
			colshape = 0;
		}
	}
	m_colshapes_list.clear();

	if(m_pDebug_renderer)
	{
		delete m_pDebug_renderer;
		m_pDebug_renderer = 0;
	}
	
	// delete trimeshes
	for(int i = 0; i < (int)m_trimesh_list.size(); i++)
	{
		btTriangleMesh* btTriMesh = m_trimesh_list[i];

		if(btTriMesh)
		{
			delete  btTriMesh; 
			btTriMesh = 0;
		}
	}
	m_trimesh_list.clear();

	// delete physics objects
	for(int i = 0; i < (int)m_pobjs_list.size(); ++i)
	{
		PhysicsObj* pobj = m_pobjs_list[i];

		if(pobj)
		{
			pobj->deinit();
			delete pobj;
			pobj = 0;
		}
	}
	m_pobjs_list.clear();

	// delete world
	delete m_dynamicsWorld;

	// delete everything else
    delete m_solver;
    delete m_dispatcher;
    delete m_collisionConfiguration;
    delete m_broadphase;
}

//creation of various collision types
btTriangleMesh* PhysicsManager::create_btTriMesh_from_mesh(Mesh* pMesh, const Vector3f& scale)
{
	if (!pMesh)
		return NULL;

	btTriangleMesh* btTriMesh = NULL;
	btTriMesh = new btTriangleMesh();
	//add collision shape to list to be deleted later
	if (btTriMesh)
		m_trimesh_list.push_back(btTriMesh);
	else
		return NULL;

	btVector3 vertice_positions[3];
	unsigned int number_of_verts = pMesh->vertices().count();
	unsigned int number_of_indices = pMesh->indices().count();
	index_t* indices = pMesh->indices().data();
	index_t index;

	vertex_t* mesh_vertices = pMesh->vertices().data();

	if(!mesh_vertices)
		return NULL;

	for(unsigned int i = 0; i < number_of_indices; i+=3)
	{
		//Apply index into vertex list
		for(int j =0; j < 3; j++)
		{
			index = indices[i+j];
			if(index > number_of_verts) continue;

			//Convert to btVector3
			btVector3 tempVec;
			tempVec.setX( (btScalar)(mesh_vertices[index].x * scale.x) );
			tempVec.setY( (btScalar)(mesh_vertices[index].y * scale.y) );
			tempVec.setZ( (btScalar)(mesh_vertices[index].z * scale.z) );

			vertice_positions[j] = tempVec;
		}
		//Add vertices to trimesh
		btTriMesh->addTriangle(vertice_positions[0], vertice_positions[1], vertice_positions[2]);
	}

	return btTriMesh;
}



void PhysicsManager::set_gravity(const Vector3f& v) 
{ 
	m_dynamicsWorld->setGravity( btVector3((btScalar)v.x,(btScalar)v.y,(btScalar)v.z) ); 
}

Vector3f PhysicsManager::get_gravity() 
{ 
	btVector3 v = m_dynamicsWorld->getGravity(); 
	return Vector3f(v.getX(),v.getY(),v.getZ()); 
}

PhysicsObj* PhysicsManager::create_object(const PhysObjInfo& info, btCollisionShape* shape, unsigned int entity_id)
{
	if(!shape)
		return NULL;

	int collisionID = (info.colId < 0 || info.colId > 15) ? 0 : 1<<(info.colId);
	std::vector<int>* colWithIds = info.colWithIds;

	int collidesWithID = 0;

	if(colWithIds && !colWithIds->empty())
	{
		for(std::vector<int>::iterator it = colWithIds->begin(); it != colWithIds->end(); ++it)
			collidesWithID |= 1<<(*it);
	}

	Matrix4x4f transformMatrix = info.m_initial_transform_matrix;
	transformMatrix.transpose();
	float matrix[16];
	matrix_to_1D_array(matrix,transformMatrix);
	btTransform trans;
	trans.setFromOpenGLMatrix(matrix);

	btScalar mass = info.mass;

	m_colshapes_list.push_back( shape );

	btDefaultMotionState* motionstate = new btDefaultMotionState(trans);

	PhysicsObj* pobj = NULL;
	if(motionstate)
	{
		btVector3 fallInertia(0,0,0);
		if(mass != 0)
			shape->calculateLocalInertia(mass,fallInertia);


		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,motionstate,shape,fallInertia);
		btRigidBody* rigidbody = NULL;
		rigidbody = new btRigidBody(fallRigidBodyCI);
		if(!rigidbody)
			return NULL;
		//rigidbody->setActivationState(DISABLE_DEACTIVATION);

		m_rbodies_list.push_back( rigidbody );

		pobj = new (std::nothrow) PhysicsObj( m_dynamicsWorld, rigidbody, entity_id, collisionID, collidesWithID);

		m_pobjs_list.push_back(pobj);
	}
	return pobj;
}

void PhysicsManager::set_enable_debug_render(const bool enable)
{
	m_bEnable_debug_render = enable;
}

bool PhysicsManager::get_enable_debug_render()
{
	return m_bEnable_debug_render;
}

//Conversion from the engine matrix to an array of 16 floats
void PhysicsManager::matrix_to_1D_array(float* outArray, const Matrix4x4f& matrix)
{
	Matrix4x4f mat = matrix;
	for(int i = 0; i < 16; i++)
	{
		outArray[i] = (float)*mat[i];
	}
}
void PhysicsManager::array_1D_to_matrix(Matrix4x4f& out_matrix, float* in_array)
{
	for(int i = 0; i < 16; i++)
	{
		*out_matrix[i] = (scalar_t)in_array[i];
	}
}

//This function must always be called right after rendering the entire 3d scene
//if the enable variable is true the debug rendering will begin otherwise nothing
//will happpen
void PhysicsManager::debug_render()
{
	m_dynamicsWorld->debugDrawWorld();
}

//creating physics objects
PhysicsObj* PhysicsManager::create_static_object_from_mesh(Mesh* pMesh, const Vector3f& scale,
														   unsigned int entity_id, 
														   const PhysObjInfo& info)
{
	btTriangleMesh* bt_tri_mesh = NULL;
	if(pMesh)
		bt_tri_mesh = create_btTriMesh_from_mesh(pMesh, scale);
	else
		return NULL;

	btBvhTriangleMeshShape* bvhShape = NULL;
	if(bt_tri_mesh)
		bvhShape = new btBvhTriangleMeshShape(bt_tri_mesh, false, true);
	else
		return NULL;
	// return create_object(info,bvhShape,entity_id); // UNCOMMENT AFTER TESTING

	// collision masking stuff
	int collisionID = (info.colId < 0 || info.colId > 15) ? 0 : 1<<(info.colId);
	std::vector<int>* colWithIds = info.colWithIds;

	int collidesWithID = 0;

	if(colWithIds && !colWithIds->empty())
	{
		for(std::vector<int>::iterator it = colWithIds->begin(); it != colWithIds->end(); ++it)
			collidesWithID |= 1<<(*it);
	}
	
	//add collision shape to be delete later on
	if(bvhShape)
		m_colshapes_list.push_back(bvhShape);
	else
		return NULL;

	float fMass = 0.0f; //Mass is zero since its static
	btVector3 localInertia(0,0,0);
	btTransform initialTransform;
	initialTransform.setIdentity();
	//Use initial matrix to apply transformation
	Matrix4x4f transformMat = info.m_initial_transform_matrix;
	//transpose matrix since its openGL matrix we need
	transformMat.transpose();
	float matrix[16];
	matrix_to_1D_array(matrix, transformMat);
	initialTransform.setFromOpenGLMatrix(matrix);

	btDefaultMotionState* temp_motion_state = new btDefaultMotionState(initialTransform);
	PhysicsObj* pobject = NULL;
	if( temp_motion_state )
	{
		btRigidBody::btRigidBodyConstructionInfo cInfo(fMass, temp_motion_state, bvhShape, localInertia);

		btRigidBody* pRigidBody = NULL;
		pRigidBody = new btRigidBody(cInfo);
		if(pRigidBody)
			pRigidBody->setContactProcessingThreshold(1e18f);
		else
			return NULL;

		//pRigidBody->setUserPointer(&entity_id);

		m_rbodies_list.push_back(pRigidBody);

		//Create physics object
		pobject  = new (std::nothrow) PhysicsObj(m_dynamicsWorld, pRigidBody, entity_id, collisionID, collidesWithID);

		if(pobject)
			m_pobjs_list.push_back(pobject);
	}
	return pobject;

}

VehicleObject* PhysicsManager::create_vehicle(const Vector3f& box_size, 
											  unsigned int entity_id,  
											  const PhysObjInfo& info,
											  const VehicleProperties& properties)

{
	// collision masking stuff
	int collisionID = (info.colId < 0 || info.colId > 15) ? 0 : 1<<(info.colId);
	std::vector<int>* colWithIds = info.colWithIds;

	int collidesWithID = 0;

	if(colWithIds && !colWithIds->empty())
	{
		for(std::vector<int>::iterator it = colWithIds->begin(); it != colWithIds->end(); ++it)
			collidesWithID |= 1<<(*it);
	}

	VehicleObject* pvehicleObject = new(std::nothrow) VehicleObject(m_dynamicsWorld, entity_id, collisionID, collidesWithID);
	if(pvehicleObject)
		pvehicleObject->init_vehicle_physics(box_size, info, properties);

	//pvehicleObject->m_rbody->setUserPointer(&entity_id);

	//Use initial matrix to apply transformation
	Matrix4x4f transformMat = info.m_initial_transform_matrix;
	//transpose matrix since its openGL matrix we need
	transformMat.transpose();
	float matrix[16];
	matrix_to_1D_array(matrix, transformMat);

	btTransform initial_Transform;
	initial_Transform.setIdentity();
	initial_Transform.setFromOpenGLMatrix(matrix);
	if(pvehicleObject)
		pvehicleObject->m_rbody->setWorldTransform(initial_Transform);

	m_pobjs_list.push_back(pvehicleObject);

	return pvehicleObject;
}

PhysicsObj* PhysicsManager::create_sphere(const PhysObjInfo& info, unsigned int ent_id, float radius)
{
	btCollisionShape* colshape = new btSphereShape((btScalar)radius); 

	return create_object(info,colshape,ent_id);
}

PhysicsObj* PhysicsManager::create_box(const PhysObjInfo& info, unsigned int ent_id, const Vector3f& sizes)
{
	btCollisionShape* colshape = new btBoxShape(btVector3((btScalar)sizes.x,(btScalar)sizes.y,(btScalar)sizes.z)); 

	return create_object(info,colshape,ent_id);
}

PhysicsObj* PhysicsManager::create_cylinder(const PhysObjInfo& info, unsigned int ent_id, float radius, float height)
{
	btCollisionShape* colshape = new btCylinderShape(btVector3((btScalar)radius,(btScalar)(height/2),(btScalar)radius)); 
	
	return create_object(info,colshape,ent_id);
}

PhysicsObj* PhysicsManager::create_plane(const PhysObjInfo& info, unsigned int ent_id, const Vector3f& normal, float offset)
{
	btCollisionShape* colshape = new btStaticPlaneShape(btVector3((btScalar)normal.x,(btScalar)normal.y,(btScalar)normal.z),(btScalar)offset);

	return create_object(info,colshape,ent_id);
}

	} /* namespace Core */
} /* namespace TEXEngine */