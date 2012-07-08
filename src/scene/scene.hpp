#ifndef TEXENGINE_SCENE_HPP_INCLUDED
#define TEXENGINE_SCENE_HPP_INCLUDED

#include <string>
#include <vector>
#include "../asset/assetManager.hpp"

namespace TEXEngine {

	namespace Core {
		class SceneManager; /* Forward declaration */
	}
	
	using Core::SceneManager;

	namespace Asset {

class Scene
{
	friend class SceneManager;
	protected:
		Scene();
		~Scene();

	public:
		// name
		const char *name();
		void name(const char *name);

		// camera
		void active_camera(const unsigned int id);
		unsigned int active_camera();

		void update();

		// RETURN CODES:
		// 0. Everything went well.
		// 2. id is already attached.
		int attach(unsigned int id);

		// RETURN CODES:
		// 0. Everything went well.
		// 2. id is not attached.
		int detach(unsigned int id);

		const std::vector<unsigned int> &entities();

	private:
		std::vector<unsigned int> m_entities;
		
		unsigned int m_camera_id;
		std::string m_name;
};

	} /* namespace Asset */
} /* namespace TEXEngine */

#endif /* TEXENGINE_SCENE_HPP_INCLUDED */