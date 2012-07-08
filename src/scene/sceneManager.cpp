#include <vector>

#include "../asset/entity.hpp"
#include "../physics/PhysicsObj.hpp"
#include "sceneManager.hpp"

using TEXEngine::Physics::PhysicsObj;
using TEXEngine::Asset::Entity;

namespace TEXEngine {
	namespace Core {

SceneManager SceneManager::m_SceneManager;

SceneManager::SceneManager()
	: m_active_scene(0)
{}

SceneManager::~SceneManager()
{
	release();
}

SceneManager& SceneManager::get()
{
	return m_SceneManager;
}

int SceneManager::create(unsigned int id)
{
	if (!id)
		return 1;

	int status = 0;

	std::map<unsigned int, Scene*>::iterator it = m_scenes.find(id);

	Scene *p = new (std::nothrow) Scene();

	if(!p)
		status = 3;

	// If the id is already mapped, release the scene.
	if(it != m_scenes.end() && status !=3) {
		delete (*it).second;
		m_scenes.erase(it);
		status = 2;
	}

	if(p)
		m_scenes[id] = p;

	return status;
}

int SceneManager::destroy(unsigned int id)
{
	std::map<unsigned int, Scene*>::iterator it = m_scenes.find(id);

	if(it != m_scenes.end()) {
		// Check if it's the currently bound scene.
		if ((*it).second == active())
			return 2;

		// If not then release it.
		delete (*it).second;
		m_scenes.erase(it);
		return 0;
	}

	return 1;
}

int SceneManager::activate(unsigned int id)
{
	// Check for 0. This means deactivate all scenes.
	if(!id) {
		m_active_scene = 0;

		// Deactivate all entities.
		const std::map<unsigned int, Entity *> &entities = AssetManager::get().entities();
		std::map<unsigned int, Entity *>::const_iterator gt;

		for (gt = entities.begin(); gt != entities.end(); gt++) {
			Entity *e = (*gt).second;
			
			if(e) {
				PhysicsObj *po = e->physics_obj();
				if (po)
					po->deactivate();
			}
		}

		return 2;
	}

	std::map<unsigned int, Scene*>::iterator it = m_scenes.find(id);

	if(it != m_scenes.end()) {
		// Deactivate all the physics objects of the current scene.
		Scene *asc = scene(m_active_scene);
	
		if (asc) {
			const std::vector<unsigned int> &a_entities = asc->entities();

			std::vector<unsigned int>::const_iterator ajt;

			for (ajt = a_entities.begin(); ajt != a_entities.end(); ajt++) {
				// Enable the physics object.
				Entity *ae = AssetManager::get().get_entity((*ajt));

				if (ae) {
					PhysicsObj *apo = ae->physics_obj();

					if (apo)
						apo->deactivate();
				}
			}
		}

		// Make it active.
		m_active_scene = id;

		// Activate the physics objects.
		Scene *sc = scene(id);
		
		if (!sc)
			return 3;

		const std::vector<unsigned int> &entities = sc->entities();

		std::vector<unsigned int>::const_iterator jt;

		for (jt = entities.begin(); jt != entities.end(); jt++) {
			// Enable the physics object.
			Entity *e = AssetManager::get().get_entity((*jt));

			if (e) {
				PhysicsObj *po = e->physics_obj();

				if (po)
					po->activate();
			}
		}

		return 0;
	}

	return 1;
}

Scene *SceneManager::active()
{
	return scene(m_active_scene);
}

Scene *SceneManager::scene(unsigned int id)
{
	std::map<unsigned int, Scene*>::iterator it = m_scenes.find(id);

	if(it != m_scenes.end())
		return (*it).second;

	return NULL;
}

void SceneManager::release()
{
	// Delete all the scenes
	for(std::map<unsigned int, Scene*>::iterator it = m_scenes.begin(); it != m_scenes.end(); it++)
		delete (*it).second;

	m_scenes.clear();
}

	} /* namespace Core */
} /* namespace TEXEngine */