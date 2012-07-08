#ifndef TEXENGINE_ASSETMANAGER_HPP_INCLUDED
#define TEXENGINE_ASSETMANAGER_HPP_INCLUDED

#include <map>
#include "../util/ncf.hpp"
#include "audioclip.hpp"
#include "light.hpp"
#include "camera.hpp"
#include "entity.hpp"
#include "mesh.hpp"
#include "texture.hpp"
#include "material.hpp"
#include "shader.hpp"
#include "../renderer/iRenderer.hpp"

// Disable: "<type> needs to have dll-interface to be used by clients"
// This warning refers to STL member variables which are private and
// therefore can be safely ignored.
#pragma warning (push)
#pragma warning (disable : 4251)

using TEXEngine::Asset::AudioClip;
using TEXEngine::Asset::Camera;
using TEXEngine::Asset::Light;
using TEXEngine::Asset::Mesh;
using TEXEngine::Asset::Texture;
using TEXEngine::Asset::Material;
using TEXEngine::Asset::Shader;
using TEXEngine::Asset::Entity;
using TEXEngine::Util::NCF1;

namespace TEXEngine {
	namespace Core {

class RenderManager; /* Forward declaration */

class AssetManager
{
	friend class RenderManager;
	public:
		// Returns a handle to the asset manager.
		static AssetManager &get();

		// RETURN CODES:
		// 0. The asset was created.
		// 1. id of 0 is invalid. Nothing done.
		// 2. The id was already mapped. The old asset was released
		//	  and a new one was created.
		// 3. Failed to create the asset.
		int create_audioclip(unsigned int id);
		int create_camera(unsigned int id);
		int create_light(unsigned int id);
		int create_mesh(unsigned int id);
		int create_texture(unsigned int id);
		int create_material(unsigned int id);
		int create_shader(unsigned int id);
		int create_entity(unsigned int id);

		// RETURN CODES:
		// 0. Everything went well.
		// 1. id of 0 is invalid. Nothing done.
		// 2. The id was already mapped. The old asset was released
		//	  and a new one was created.
		// 3. Failed to create the asset.
		// 4. ncf equal to NULL.
		int create_audioclip(const NCF1 *ncf);
		int create_camera(const NCF1 *ncf);
		int create_light(const NCF1 *ncf);
		int create_mesh(const NCF1 *ncf);
		int create_texture(const NCF1 *ncf);
		int create_material(const NCF1 *ncf);
		int create_shader(const NCF1 *ncf);
		int create_entity(const NCF1 *ncf);

		// RETURN CODES:
		// 0. The asset was released.
		// 1. The id is not mapped.
		int destroy_audioclip(unsigned int id);
		int destroy_camera(unsigned int id);
		int destroy_light(unsigned int id);
		int destroy_mesh(unsigned int id);
		int destroy_texture(unsigned int id);
		int destroy_material(unsigned int id);
		int destroy_shader(unsigned int id);
		int destroy_entity(unsigned int id);

		// NOTES:
		// If the id doesn't map to an asset, NULL is returned.
		AudioClip *get_audioclip(unsigned int id);
		Camera *get_camera(unsigned int id);
		Light *get_light(unsigned int id);
		Mesh *get_mesh(unsigned int id);
		Texture *get_texture(unsigned int id);
		Material *get_material(unsigned int id);
		Shader *get_shader(unsigned int id);
		Entity *get_entity(unsigned int id);

		const std::map<unsigned int, Light*> &lights() const;
		const std::map<unsigned int, Entity*> &entities() const;

		// Load assets from an NCF script.
		// RETURN CODES:
		// 0. Everything went well.
		// 1. ncf equal to NULL.
		// 2. Unrecognised resource class.
		int load(const NCF1 *ncf);
		void clear();
	
	protected:
		void setup_renderer(IRenderer *renderer);

	private:
		AssetManager();
		~AssetManager();

		void release();

		std::map<unsigned int, AudioClip*> m_audioclips;
		std::map<unsigned int, Camera*> m_cameras;
		std::map<unsigned int, Light*> m_lights;
		std::map<unsigned int, Mesh*> m_meshes;
		std::map<unsigned int, Texture*> m_textures;
		std::map<unsigned int, Material*> m_materials;
		std::map<unsigned int, Shader*> m_shaders;
		std::map<unsigned int, Entity*> m_entities;

		static AssetManager m_asset_manager;

		IRenderer *m_renderer;
};

	} /* namespace Core */
} /* namespace TEXEngine */

#pragma warning (pop)

#endif /* TEXENGINE_LOGMANAGER_HPP_INCLUDED */