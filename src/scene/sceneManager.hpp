#ifndef TEXENGINE_SCENEMANAGER_HPP_INCLUDED
#define TEXENGINE_SCENEMANAGER_HPP_INCLUDED

#include <map>
#include "scene.hpp"

using TEXEngine::Asset::Scene;

namespace TEXEngine {
	namespace Core {

class SceneManager
{
	public:
		static SceneManager& get();
		
		// RETURN CODES:
		// 0. Everything went well.
		// 1. id of 0 is invalid. Nothing done.
		// 2. The id was already mapped. The old scene was released
		//	  and a new one was created.
		// 3. Failed to create the scene.
		int create(unsigned int id);

		// RETURN CODES:
		// 0. The scene was released.
		// 1. The id is not mapped.
		// 2. The scene is currently active and can't be released.
		int destroy(unsigned int id);


		// Set the active scene.
		// RETURN CODES:
		// 0. Everything went well.
		// 1. The scene doesn't exist.
		// 2. Deactivated all scenes. (You gave an id of 0).
		// 3. NULL scene pointer.
		int activate(unsigned int id);

		// Get a pointer to the active scene.
		// NOTES:
		// If this return NULL it means no scene is currently active;
		Scene *active();

		// Get a pointer to a scene by id.
		// NOTES:
		// If this return NULL it means that the id is not mapped;
		Scene *scene(unsigned int id);
		
	private:
		SceneManager();
		~SceneManager();

		void release();

		unsigned int m_active_scene;
		std::map<unsigned int, Scene*> m_scenes;

		static SceneManager m_SceneManager;
};

	} /* namespace Core */
} /* namespace TEXEngine */

#endif /* TEXENGINE_SCENEMANAGER_HPP_INCLUDED */