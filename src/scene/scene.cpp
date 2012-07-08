#include "../log/logmanager.hpp"
#include "scene.hpp"

using TEXEngine::Core::LogManager;

namespace TEXEngine {
	namespace Asset {

Scene::Scene()
	: m_camera_id(0)
{}

Scene::~Scene()
{}

void Scene::name(const char *name)
{
	m_name = name;
}

const char *Scene::name()
{
	return m_name.c_str();
}

void Scene::active_camera(const unsigned int id)
{
	m_camera_id = id;
}

unsigned int Scene::active_camera()
{
	return m_camera_id;
}

const std::vector<unsigned int> &Scene::entities()
{
	return m_entities;
}

int Scene::attach(unsigned int id)
{
	std::vector<unsigned int>::iterator it;
	// Check if the entity is already attached.
	for (it = m_entities.begin(); it != m_entities.end(); it++) 
		if ((*it) == id) {
			LogManager::get().log_warning("The entity [%i] has already been attached to [%s].", id, m_name.c_str());
			return 1;
		}

	// Attach it.
	m_entities.push_back(id);
	LogManager::get().log_message("Attached entity [%i] to scene [%s].", id, m_name.c_str());

	return 0;
}

int Scene::detach(unsigned int id)
{
	std::vector<unsigned int>::iterator it;
	// Check if the entity is already attached.
	for (it = m_entities.begin(); it != m_entities.end(); it++) 
		if ((*it) == id) {
			m_entities.erase(it);
			LogManager::get().log_message("Detached entity [%i] from scene [%s].", id, m_name.c_str());
			return 0;
		}

	LogManager::get().log_warning("The entity [%i] has not been attached to [%s].", id, m_name.c_str());
	return 1;
}

	} /* namespace Asset */
} /* namespace TEXEngine */