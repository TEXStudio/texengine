#include "../util/nstring.hpp"
#include "../util/obj.hpp"
#include "../util/ppm.hpp"
#include "../util/obj.hpp"
#include "../util/genrplane.hpp"
#include "../util/genrcylinder.hpp"
#include "../util/genrdisk.hpp"
#include "../util/genrsphere.hpp"
#include "../util/xortex.hpp"
#include "../util/gradient.hpp"
#include "../util/alphamap.hpp"
#include "../util/chroma.hpp"
#include "../util/multiply.hpp"
#include "../util/heightmap.hpp"
#include "../util/translate.hpp"
#include "../util/rotate.hpp"
#include "../util/scale.hpp"
#include "../util/calcnorm.hpp"
#include "../util/calctangent.hpp"
#include "../util/flipfaces.hpp"
#include "../log/logmanager.hpp"
#include "assetManager.hpp"
#include "../asset/shaderManager.hpp"

using namespace TEXEngine::Util::NString;

using TEXEngine::Math::Vector3f;
using TEXEngine::Util::ColorRGBAf;
using TEXEngine::Util::Generator::gradient;
using TEXEngine::Util::Generator::xortex;
using TEXEngine::Util::Generator::cylinder;
using TEXEngine::Util::Generator::disk;
using TEXEngine::Util::Generator::plane;
using TEXEngine::Util::Generator::sphere;
using TEXEngine::Util::IO::Import::ppm_raw;
using TEXEngine::Util::IO::Import::obj;
using TEXEngine::Util::Mutator::alpha_map;
using TEXEngine::Util::Mutator::chroma_key;
using TEXEngine::Util::Mutator::multiply;
using TEXEngine::Util::Mutator::heightmap;
using TEXEngine::Util::Mutator::scale;
using TEXEngine::Util::Mutator::rotate;
using TEXEngine::Util::Mutator::translate;
using TEXEngine::Util::Mutator::calc_normals;
using TEXEngine::Util::Mutator::calc_tangent;
using TEXEngine::Util::Mutator::flip_faces;
using TEXEngine::Asset::TEXENG_AUDIO_FX;
using TEXEngine::Asset::TEXENG_AUDIO_MUSIC;
using TEXEngine::Asset::TEXENG_TEXTURE_TYPE;
using TEXEngine::Asset::TEXENG_TEXTURE_COLORMAP;
using TEXEngine::Asset::TEXENG_TEXTURE_HEIGHTMAP;
using TEXEngine::Asset::TEXENG_TEXTURE_NORMALMAP1;
using TEXEngine::Asset::TEXENG_TEXTURE_NORMALMAP2;
using TEXEngine::Asset::TEXENG_TEXTURE_BLENDMAP;
using TEXEngine::Core::LogManager;
using TEXEngine::Core::ShaderManager;

namespace TEXEngine {	
	namespace Core {

AssetManager AssetManager::m_asset_manager;

AssetManager::AssetManager()
{}

AssetManager::~AssetManager()
{
	release();
}

AssetManager &AssetManager::get()
{
	return m_asset_manager;
}

void AssetManager::release()
{
	// Delete all the audioclips
	for(std::map<unsigned int, AudioClip*>::iterator it = m_audioclips.begin(); it != m_audioclips.end(); it++)
		delete (*it).second;

	m_audioclips.clear();

	// Delete all the cameras
	for(std::map<unsigned int, Camera*>::iterator it = m_cameras.begin(); it != m_cameras.end(); it++)
		delete (*it).second;

	m_cameras.clear();

	// Delete all the lights
	for(std::map<unsigned int, Light*>::iterator it = m_lights.begin(); it != m_lights.end(); it++)
		delete (*it).second;

	m_lights.clear();

	// Delete all the meshes
	for(std::map<unsigned int, Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); it++)
		delete (*it).second;

	m_meshes.clear();

	// Delete all the textures
	for(std::map<unsigned int, Texture*>::iterator it = m_textures.begin(); it != m_textures.end(); it++)
		delete (*it).second;

	m_textures.clear();

	// Delete all the materials
	for(std::map<unsigned int, Material*>::iterator it = m_materials.begin(); it != m_materials.end(); it++)
		delete (*it).second;

	m_materials.clear();

	// Delete all the shaders
	for(std::map<unsigned int, Shader*>::iterator it = m_shaders.begin(); it != m_shaders.end(); it++)
		delete (*it).second;

	m_shaders.clear();

	// Delete all the entities
	for(std::map<unsigned int, Entity*>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
		delete (*it).second;

	m_entities.clear();
}


int AssetManager::create_audioclip(unsigned int id)
{
	if (!id)
		return 1;

	int status = 0;

	std::map<unsigned int, AudioClip*>::iterator it = m_audioclips.find(id);

	AudioClip *p = new (std::nothrow) AudioClip();

	if(!p)
		status = 3;

	// If the id is already mapped, release the asset.
	if(it != m_audioclips.end() && status != 3) {
		delete (*it).second;
		m_audioclips.erase(it);
		status = 2;
	}

	if(p)
		m_audioclips[id] = p;

	return status;
}

int AssetManager::create_camera(unsigned int id)
{
	if (!id)
		return 1;

	int status = 0;

	std::map<unsigned int, Camera*>::iterator it = m_cameras.find(id);

	Camera *p = new (std::nothrow) Camera();

	if(!p)
		status = 3;

	// If the id is already mapped, release the asset.
	if(it != m_cameras.end() && status != 3) {
		delete (*it).second;
		m_cameras.erase(it);
		status = 2;
	}

	if(p)
		m_cameras[id] = p;

	return status;
}

int AssetManager::create_light(unsigned int id)
{
	if (!id)
		return 1;

	int status = 0;

	std::map<unsigned int, Light*>::iterator it = m_lights.find(id);

	Light *p = new (std::nothrow) Light();

	if(p)
		status = 3;

	// If the id is already mapped, release the asset.
	if(it != m_lights.end() && status != 3) {
		delete (*it).second;
		m_lights.erase(it);
		status = 2;
	}

	if(p)
		m_lights[id] = p;

	return status;
}

int AssetManager::create_mesh(unsigned int id)
{
	if (!id)
		return 1;

	int status = 0;

	std::map<unsigned int, Mesh*>::iterator it = m_meshes.find(id);

	Mesh *p = new (std::nothrow) Mesh();

	if(!p)
		status = 3;

	// If the id is already mapped, release the asset.
	if(it != m_meshes.end() && status != 3) {
		delete (*it).second;
		m_meshes.erase(it);
		status = 2;
	}

	if(p)
		m_meshes[id] = p;

	return status;
}

int AssetManager::create_texture(unsigned int id)
{
	if (!id)
		return 1;

	int status = 0;

	std::map<unsigned int, Texture*>::iterator it = m_textures.find(id);

	Texture *p = new (std::nothrow) Texture();

	if(!p)
		status = 3;

	// If the id is already mapped, release the asset.
	if(it != m_textures.end() && status != 3) {
		delete (*it).second;
		m_textures.erase(it);
		status = 2;
	}

	if(p)
		m_textures[id] = p;

	return status;
}

int AssetManager::create_material(unsigned int id)
{
	if (!id)
		return 1;
	
	int status = 0;

	std::map<unsigned int, Material*>::iterator it = m_materials.find(id);

	Material *p = new (std::nothrow) Material();

	if(!p)
		status = 3;

	// If the id is already mapped, release the asset.
	if(it != m_materials.end() && status != 3) {
		delete (*it).second;
		m_materials.erase(it);
		status = 2;
	}

	if(p)
		m_materials[id] = p;

	return status;
}

int AssetManager::create_shader(unsigned int id)
{
	if (!id)
		return 1;

	int status = 0;

	std::map<unsigned int, Shader*>::iterator it = m_shaders.find(id);

	Shader *p = new (std::nothrow) Shader();

	if(!p)
		status = 3;

	// If the id is already mapped, release the asset.
	if(it != m_shaders.end() && status != 3) {
		delete (*it).second;
		m_shaders.erase(it);
		status = 2;
	}

	if(p)
		m_shaders[id] = p;

	return status;
}

int AssetManager::create_entity(unsigned int id)
{
	if (!id)
		return 1;

	int status = 0;

	std::map<unsigned int, Entity*>::iterator it = m_entities.find(id);

	Entity *p = new (std::nothrow) Entity();

	if(!p)
		status = 3;

	// If the id is already mapped, release the asset.
	if(it != m_entities.end() && status != 3) {
		delete (*it).second;
		m_entities.erase(it);
		status = 2;
	}

	if(p)
		m_entities[id] = p;

	return status;
}

int AssetManager::destroy_audioclip(unsigned int id)
{
	std::map<unsigned int, AudioClip*>::iterator it = m_audioclips.find(id);

	if(it != m_audioclips.end()) {
		delete (*it).second;
		m_audioclips.erase(it);
		return 0;		
	}

	return 1;
}

int AssetManager::destroy_camera(unsigned int id)
{
	std::map<unsigned int, Camera*>::iterator it = m_cameras.find(id);

	if(it != m_cameras.end()) {
		delete (*it).second;
		m_cameras.erase(it);
		return 0;		
	}

	return 1;
}

int AssetManager::destroy_light(unsigned int id)
{
	std::map<unsigned int, Light*>::iterator it = m_lights.find(id);

	if(it != m_lights.end()) {
		delete (*it).second;
		m_lights.erase(it);
		return 0;		
	}

	return 1;
}

int AssetManager::destroy_mesh(unsigned int id)
{
	std::map<unsigned int, Mesh*>::iterator it = m_meshes.find(id);

	if(it != m_meshes.end()) {
		if (m_renderer)
			m_renderer->deinitiate_mesh((*it).second);

		delete (*it).second;
		m_meshes.erase(it);

		return 0;		
	}

	return 1;
}

int AssetManager::destroy_texture(unsigned int id)
{
	std::map<unsigned int, Texture*>::iterator it = m_textures.find(id);

	if(it != m_textures.end()) {
		if (m_renderer)
			m_renderer->deinitiate_texture((*it).second);

		delete (*it).second;
		m_textures.erase(it);
		return 0;		
	}

	return 1;
}

int AssetManager::destroy_material(unsigned int id)
{
	std::map<unsigned int, Material*>::iterator it = m_materials.find(id);

	if(it != m_materials.end()) {
		delete (*it).second;
		m_materials.erase(it);
		return 0;		
	}

	return 1;
}

int AssetManager::destroy_shader(unsigned int id)
{
	std::map<unsigned int, Shader*>::iterator it = m_shaders.find(id);

	if(it != m_shaders.end()) {
		delete (*it).second;
		m_shaders.erase(it);
		return 0;		
	}

	return 1;
}

int AssetManager::destroy_entity(unsigned int id)
{
	std::map<unsigned int, Entity*>::iterator it = m_entities.find(id);

	if(it != m_entities.end()) {
		delete (*it).second;
		m_entities.erase(it);
		return 0;		
	}

	return 1;
}

// Asset accessor functions.
AudioClip *AssetManager::get_audioclip(unsigned int id)
{
	std::map<unsigned int, AudioClip*>::iterator it = m_audioclips.find(id);

	if(it != m_audioclips.end()) {
		return (*it).second;		
	}

	return NULL;
}

Camera *AssetManager::get_camera(unsigned int id)
{
	std::map<unsigned int, Camera*>::iterator it = m_cameras.find(id);

	if(it != m_cameras.end()) {
		return (*it).second;		
	}

	return NULL;
}

Light *AssetManager::get_light(unsigned int id)
{
	std::map<unsigned int, Light*>::iterator it = m_lights.find(id);

	if(it != m_lights.end()) {
		return (*it).second;		
	}

	return NULL;
}

Mesh *AssetManager::get_mesh(unsigned int id)
{
	std::map<unsigned int, Mesh*>::iterator it = m_meshes.find(id);

	if(it != m_meshes.end()) {
		return (*it).second;		
	}

	return NULL;
}

Texture *AssetManager::get_texture(unsigned int id)
{
	std::map<unsigned int, Texture*>::iterator it = m_textures.find(id);

	if(it != m_textures.end()) {
		return (*it).second;		
	}

	return NULL;
}

Material *AssetManager::get_material(unsigned int id)
{
	std::map<unsigned int, Material*>::iterator it = m_materials.find(id);

	if(it != m_materials.end()) {
		return (*it).second;		
	}

	return NULL;
}

Shader *AssetManager::get_shader(unsigned int id)
{
	std::map<unsigned int, Shader*>::iterator it = m_shaders.find(id);

	if(it != m_shaders.end()) {
		return (*it).second;		
	}

	return NULL;
}

Entity *AssetManager::get_entity(unsigned int id)
{
	std::map<unsigned int, Entity*>::iterator it = m_entities.find(id);

	if(it != m_entities.end()) {
		return (*it).second;		
	}

	return NULL;
}

const std::map<unsigned int, Entity*> &AssetManager::entities() const
{
	return m_entities;
}

const std::map<unsigned int, Light*> &AssetManager::lights() const
{
	return m_lights;
}

int AssetManager::load(const NCF1 *ncf)
{
	if(!ncf)
		return 1;

	int retcode = 0;
	
	// Load all the resources
	LogManager::get().log_message("Loading resources..");

	LogManager::get().log_warning("%i groups", ncf->group("resource")->count_groups());
	
	for (unsigned int i = 0; i < ncf->group("resource")->count_groups(); i++) {
		std::string type = ncf->group("resource")->group((unsigned int)i)->name();
		LogManager::get().log_warning("Loading class [ %s ]..", type.c_str());
	
		if (!(type.compare("audioclip"))) {
			LogManager::get().log_message("Loading audioclips..");
			for (unsigned int j = 0; j < ncf->group("resource")->group((unsigned int)i)->count_groups(); j++)
				create_audioclip(ncf->group("resource")->group((unsigned int)i)->group((unsigned int)j));
		}
		else if (!(type.compare("camera"))) {
			LogManager::get().log_message("Loading cameras..");
			for (unsigned int j = 0; j < ncf->group("resource")->group((unsigned int)i)->count_groups(); j++)
				create_camera(ncf->group("resource")->group((unsigned int)i)->group((unsigned int)j));
		}
		else if (!(type.compare("light"))) {
			LogManager::get().log_message("Loading lights..");
			for (unsigned int j = 0; j < ncf->group("resource")->group((unsigned int)i)->count_groups(); j++)
				create_light(ncf->group("resource")->group((unsigned int)i)->group((unsigned int)j));
		}
		else if (!(type.compare("mesh"))) {
			LogManager::get().log_message("Loading meshes..");
			for (unsigned int j = 0; j < ncf->group("resource")->group((unsigned int)i)->count_groups(); j++)
				create_mesh(ncf->group("resource")->group((unsigned int)i)->group((unsigned int)j));
		}
		else if (!(type.compare("texture"))) {
			LogManager::get().log_warning("%i groups", ncf->group("resource")->group((unsigned int)i)->count_groups());
			LogManager::get().log_message("Loading textures..");
			for (unsigned int j = 0; j < ncf->group("resource")->group((unsigned int)i)->count_groups(); j++)
				create_texture(ncf->group("resource")->group((unsigned int)i)->group((unsigned int)j));
		}
		else if (!(type.compare("material"))) {
			LogManager::get().log_message("Loading materials..");
			for (unsigned int j = 0; j < ncf->group("resource")->group((unsigned int)i)->count_groups(); j++)
				create_material(ncf->group("resource")->group((unsigned int)i)->group((unsigned int)j));
		}
		else if (!(type.compare("shader"))) {
			LogManager::get().log_message("Loading shaders.. %i", ncf->group("resource")->group((unsigned int)i)->count_groups());
			for (unsigned int j = 0; j < ncf->group("resource")->group((unsigned int)i)->count_groups(); j++){
				LogManager::get().log_message("creating shader");
				int ret = create_shader(ncf->group("resource")->group((unsigned int)i)->group((unsigned int)j));
				LogManager::get().log_message("craeted shader with %i", ret);
			}
		}
		else if (!(type.compare("entity"))) {
			LogManager::get().log_message("Loading entities..");
			for (unsigned int j = 0; j < ncf->group("resource")->group((unsigned int)i)->count_groups(); j++)
				create_entity(ncf->group("resource")->group((unsigned int)i)->group((unsigned int)j));
		}	
		else
			retcode = 2;
	}

	return retcode;
}

int AssetManager::create_audioclip(const NCF1 *ncf)
{
	if(!ncf)
		return 4;

	LogManager::get().log_message("Creating audioclip [%s]..", ncf->name());

	unsigned int id = to_int(ncf->get("id"));
	
	if (!id) {
		LogManager::get().log_error("Invalid audioclip id (=0)..");
		return 1;
	}

	int status = 0;

	std::map<unsigned int, AudioClip*>::iterator it = m_audioclips.find(id);

	AudioClip *p = new (std::nothrow) AudioClip();

	if(!p)
		status = 3;

	// If the id is already mapped, release the asset.
	if(it != m_audioclips.end() && status != 3) {
		delete (*it).second;
		m_audioclips.erase(it);
		status = 2;
	}

	if(p) {
		m_audioclips[id] = p;
	
		p->type = std::string(ncf->get("type")).compare("music") ? 
			TEXENG_AUDIO_FX : TEXENG_AUDIO_MUSIC;

		p->filename = ncf->get("data");

		p->position = Vector3f(
			to_float(ncf->group("position")->get("x")),
			to_float(ncf->group("position")->get("y")),
			to_float(ncf->group("position")->get("z")));

		p->velocity = Vector3f(
			to_float(ncf->group("velocity")->get("x")),
			to_float(ncf->group("velocity")->get("y")),
			to_float(ncf->group("velocity")->get("z")));
	}

	return status;
}

int AssetManager::create_camera(const NCF1 *ncf)
{
	if(!ncf)
		return 4;
	
	LogManager::get().log_message("Creating camera [%s]..", ncf->name());

	unsigned int id = to_int(ncf->get("id"));
	
	if (!id) {
		LogManager::get().log_error("Invalid camera id (=0)..");
		return 1;
	}

	int status = 0;

	std::map<unsigned int, Camera*>::iterator it = m_cameras.find(id);

	Camera *p = new (std::nothrow) Camera();

	if(!p)
		status = 3;

	// If the id is already mapped, release the asset.
	if(it != m_cameras.end() && status != 3) {
		delete (*it).second;
		m_cameras.erase(it);
		status = 2;
	}

	if(p) {
		m_cameras[id] = p;
	
		p->position(Vector3f(
			to_float(ncf->group("position")->get("x")),
			to_float(ncf->group("position")->get("y")),
			to_float(ncf->group("position")->get("z"))));

		p->target(Vector3f(
			to_float(ncf->group("target")->get("x")),
			to_float(ncf->group("target")->get("y")),
			to_float(ncf->group("target")->get("z"))));

		p->up(Vector3f(
			to_float(ncf->group("up")->get("x")),
			to_float(ncf->group("up")->get("y")),
			to_float(ncf->group("up")->get("z"))));
	}
	return status;
}

int AssetManager::create_light(const NCF1 *ncf)
{
	if(!ncf)
		return 4;

	LogManager::get().log_message("Creating light [%s]..", ncf->name());

	unsigned int id = to_int(ncf->get("id"));
	
	if (!id) {
		LogManager::get().log_error("Invalid light id (=0)..");
		return 1;
	}

	int status = 0;

	std::map<unsigned int, Light*>::iterator it = m_lights.find(id);

	Light *p = new (std::nothrow) Light();

	if(!p)
		status = 3;

	// If the id is already mapped, release the asset.
	if(it != m_lights.end() && status != 3) {
		delete (*it).second;
		m_lights.erase(it);
		status = 2;
	}

	if(p) {
		m_lights[id] = p;
	
		p->position(Vector3f(
			to_float(ncf->group("position")->get("x")),
			to_float(ncf->group("position")->get("y")),
			to_float(ncf->group("position")->get("z"))));

		p->target(Vector3f(
			to_float(ncf->group("target")->get("x")),
			to_float(ncf->group("target")->get("y")),
			to_float(ncf->group("target")->get("z"))));

		p->ambient(ColorRGBAf(
			to_float(ncf->group("ambient")->get("r")),
			to_float(ncf->group("ambient")->get("g")),
			to_float(ncf->group("ambient")->get("b")),
			to_float(ncf->group("ambient")->get("a"))));

		p->diffuse(ColorRGBAf(
			to_float(ncf->group("diffuse")->get("r")),
			to_float(ncf->group("diffuse")->get("g")),
			to_float(ncf->group("diffuse")->get("b")),
			to_float(ncf->group("diffuse")->get("a"))));

		p->specular(ColorRGBAf(
			to_float(ncf->group("specular")->get("r")),
			to_float(ncf->group("specular")->get("g")),
			to_float(ncf->group("specular")->get("b")),
			to_float(ncf->group("specular")->get("a"))));

		p->index(to_int(ncf->get("index")));

		p->cutoff(to_float(ncf->get("cutoff")));
		p->exponent(to_float(ncf->get("exponent")));
		p->constn_attenuation(to_float(ncf->get("constant_attenuation")));
		p->linear_attenuation(to_float(ncf->get("linear_attenuation")));
		p->quadrc_attenuation(to_float(ncf->get("quadratic_attenuation")));

		bool is_active = to_bool(ncf->get("is_active"));
		
		if (is_active)
			p->activate();
		else
			p->deactivate();

		std::string type = ncf->get("type");

		if(!type.compare("directional"))
			p->type(TEXENG_LIGHT_DIRECTIONAL);
		else if(!type.compare("point"))
			p->type(TEXENG_LIGHT_POINT);
		else
			p->type(TEXENG_LIGHT_SPOT);
	}
	return status;
}

int AssetManager::create_mesh(const NCF1 *ncf)
{
	if(!ncf)
		return 4;

	LogManager::get().log_message("Creating mesh [%s]..", ncf->name());

	unsigned int id = to_int(ncf->get("id"));
	
	if (!id) {
		LogManager::get().log_error("Invalid mesh id (=0)..");
		return 1;
	}

	int status = 0;

	std::map<unsigned int, Mesh*>::iterator it = m_meshes.find(id);

	Mesh *p = new (std::nothrow) Mesh();

	if(!p)
		status = 3;

	// If the id is already mapped, release the asset.
	if(it != m_meshes.end() && status != 3) {
		delete (*it).second;
		m_meshes.erase(it);
		status = 2;
	}

	if(p) {
		m_meshes[id] = p;

		std::string type = ncf->get("type");

		if (!type.compare("external")) {
			obj(ncf->get("data"), p);
		}
		else if  (!type.compare("procedural")) {
			std::string gclass = ncf->get("class");

			float b_radius = to_float(ncf->get("b_radius"));
			float t_radius = to_float(ncf->get("t_radius"));
			float height = to_float(ncf->get("height"));
			unsigned int stacks = to_int(ncf->get("stacks"));
			unsigned int slices = to_int(ncf->get("slices"));
			unsigned int b_rings = to_int(ncf->get("b_rings"));
			unsigned int t_rings = to_int(ncf->get("t_rings"));
			float ib_radius = to_float(ncf->get("ib_radius"));
			float it_radius = to_float(ncf->get("it_radius"));
			unsigned int rings = to_int(ncf->get("rings"));
			float radius = to_float(ncf->get("radius"));
			float i_radius = to_float(ncf->get("i_radius"));
			float size = to_float(ncf->get("size"));
			float uv_scale = to_float(ncf->get("uv_scale"));

			if(!gclass.compare("cylinder"))
				cylinder(p, b_radius, t_radius, height, stacks, slices,
					b_rings, t_rings, ib_radius, it_radius);
			else if(!gclass.compare("disk"))
				disk(p, slices, rings, radius, i_radius);
			else if(!gclass.compare("plane"))
				plane(p, size, slices, uv_scale);
			else if(!gclass.compare("sphere"))
				sphere(p, radius, slices, stacks);
		}

		if(ncf->query_property("heightmap")) {
			float base = to_float(ncf->get("heightmap_base"));
			float mult = to_float(ncf->get("heightmap_multiplier"));

			heightmap(p, ncf->get("heightmap"), base, mult);
		}

		if(ncf->query_group("rotate")){
			const Vector3f v(to_float(ncf->group("rotate")->get("x")),
				to_float(ncf->group("rotate")->get("y")),
				to_float(ncf->group("rotate")->get("z")));

			rotate(p, v);
		}

		if(ncf->query_group("scale")){
			const Vector3f v(to_float(ncf->group("scale")->get("x")),
				to_float(ncf->group("scale")->get("y")),
				to_float(ncf->group("scale")->get("z")));

			scale(p, to_float(ncf->get("scale_factor")), v);
		}

		if(ncf->query_group("translate")){
			const Vector3f v(to_float(ncf->group("translate")->get("x")),
				to_float(ncf->group("translate")->get("y")),
				to_float(ncf->group("translate")->get("z")));

			translate(p, v);
		}
		
		if(to_bool(ncf->get("flip_faces"))) {
			LogManager::get().log_message(" oooooooooooooooooo FLIPZORZ %s", ncf->get("flip_faces"));
			flip_faces(p);
		}

		if  (!type.compare("procedural")) {
			calc_normals(p);
		}

		calc_tangent(p);
	}

	return status;
}

int AssetManager::create_texture(const NCF1 *ncf)
{
	if(!ncf)
		return 4;

	LogManager::get().log_message("Creating texture [%s]..", ncf->name());

	unsigned int id = to_int(ncf->get("id"));
	
	if (!id) {
		LogManager::get().log_error("Invalid texture id (=0)..");
		return 1;
	}

	int status = 0;

	std::map<unsigned int, Texture*>::iterator it = m_textures.find(id);

	Texture *p = new (std::nothrow) Texture();

	if(!p)
		status = 3;

	// If the id is already mapped, release the asset.
	if(it != m_textures.end() && status != 3) {
		delete (*it).second;
		m_textures.erase(it);
		status = 2;
	}

	if(p) {
		m_textures[id] = p;
	
		std::string texclass = ncf->get("class");
		
		if (!texclass.compare("sprite")) p->type(TEXENG_TEXTURE_SPRITE);
		else if (!texclass.compare("font")) p->type(TEXENG_TEXTURE_FONT);
		else if (!texclass.compare("colormap")) p->type(TEXENG_TEXTURE_COLORMAP);
		else if (!texclass.compare("heightmap")) p->type(TEXENG_TEXTURE_HEIGHTMAP);
		else if (!texclass.compare("normalmap1")) p->type(TEXENG_TEXTURE_NORMALMAP1);
		else if (!texclass.compare("normalmap2")) p->type(TEXENG_TEXTURE_NORMALMAP2);
		else if (!texclass.compare("blendmap")) p->type(TEXENG_TEXTURE_BLENDMAP);
		else p->type(TEXENG_TEXTURE_GENERIC);
	
		std::string type = ncf->get("type");

		if (!type.compare("external")) {
			ppm_raw(ncf->get("data"), &p->img);
		}
		else if  (!type.compare("procedural")) {
			unsigned int w = to_int(ncf->group("dimensions")->get("x"));
			unsigned int h = to_int(ncf->group("dimensions")->get("y"));

			std::string genr = ncf->get("generator");

			if (!genr.compare("xor"))
				xortex(&p->img, w, h);
			else if  (!genr.compare("gradient")) {
				Vector2f dir = Vector2f(
					to_float(ncf->group("grad_direction")->get("x")),
					to_float(ncf->group("grad_direction")->get("y")));

				ColorRGBAf cola(
					to_float(ncf->group("grad_color_a")->get("r")),
					to_float(ncf->group("grad_color_a")->get("g")),
					to_float(ncf->group("grad_color_a")->get("b")),
					to_float(ncf->group("grad_color_a")->get("a")));

				ColorRGBAf colb(
					to_float(ncf->group("grad_color_b")->get("r")),
					to_float(ncf->group("grad_color_b")->get("g")),
					to_float(ncf->group("grad_color_b")->get("b")),
					to_float(ncf->group("grad_color_b")->get("a")));

				bool use_alpha(to_bool(ncf->get("use_alpha")));

				std::string grad_type = ncf->get("grad_type");

				TEXENG_GRADIENT_TYPE gtype = TEXENG_GRADIENT_RADIAL_A;

				if(!grad_type.compare("perpixel"))
					gtype = TEXENG_GRADIENT_PERPIXEL;

				gradient(&p->img, w, h, cola, colb, dir, use_alpha, gtype);
			}
		}

		if (ncf->query_property("alphamap")) {
			std::string alphamap = ncf->get("alphamap");
		
			if (alphamap.size()) {
				Image alpha;
				ppm_raw(alphamap.c_str(), &alpha);
				alpha_map(&p->img, &alpha);
			}
		}

		p->offset(Vector2f(
			to_float(ncf->group("uv_offset")->get("u")), 
			to_float(ncf->group("uv_offset")->get("v"))));
		
		if (ncf->query_group("chroma_key")) {
			ColorRGBAf chrkey(
				to_float(ncf->group("chroma_key")->get("r")),
				to_float(ncf->group("chroma_key")->get("g")),
				to_float(ncf->group("chroma_key")->get("b")),
				to_float(ncf->group("chroma_key")->get("a")));

			float chroma_key_opacity = to_float(ncf->get("chroma_key_opacity"));
			float chroma_key_threshold = to_float(ncf->get("chroma_key_threshold"));

			chroma_key(&p->img, chrkey, chroma_key_opacity, chroma_key_threshold);
		}

		if (ncf->query_property("multiply")) {
			Image mult;
			ppm_raw(ncf->get("multiply"), &mult);
			multiply(&p->img, &mult);
		}
	}
	return status;
}

int AssetManager::create_material(const NCF1 *ncf)
{
	if(!ncf)
		return 4;

	LogManager::get().log_message("Creating material [%s]..", ncf->name());

	unsigned int id = to_int(ncf->get("id"));
	
	if (!id) {
		LogManager::get().log_error("Invalid material id (=0)..");
		return 1;
	}

	int status = 0;

	std::map<unsigned int, Material*>::iterator it = m_materials.find(id);

	Material *p = new (std::nothrow) Material();

	if(!p)
		status = 3;

	// If the id is already mapped, release the asset.
	if(it != m_materials.end() && status != 3) {
		delete (*it).second;
		m_materials.erase(it);
		status = 2;
	}

	if(p) {
		m_materials[id] = p;
	
		p->ambient(ColorRGBAf(
			to_float(ncf->group("ambient")->get("r")),
			to_float(ncf->group("ambient")->get("g")),
			to_float(ncf->group("ambient")->get("b")),
			to_float(ncf->group("ambient")->get("a"))));

		p->diffuse(ColorRGBAf(
			to_float(ncf->group("diffuse")->get("r")),
			to_float(ncf->group("diffuse")->get("g")),
			to_float(ncf->group("diffuse")->get("b")),
			to_float(ncf->group("diffuse")->get("a"))));

		p->specular(ColorRGBAf(
			to_float(ncf->group("specular")->get("r")),
			to_float(ncf->group("specular")->get("g")),
			to_float(ncf->group("specular")->get("b")),
			to_float(ncf->group("specular")->get("a"))));

		p->emission(ColorRGBAf(
			to_float(ncf->group("emission")->get("r")),
			to_float(ncf->group("emission")->get("g")),
			to_float(ncf->group("emission")->get("b")),
			to_float(ncf->group("emission")->get("a"))));

		p->shininess(to_float(ncf->get("shininess")));
	}
	return status;
}

int AssetManager::create_shader(const NCF1 *ncf)
{
	if(!ncf)
		return 4;

	LogManager::get().log_message("Creating shader [%s]..", ncf->name());

	unsigned int id = to_int(ncf->get("id"));
	
	if (!id) {
		LogManager::get().log_error("Invalid shader id (=0)..");
		return 1;
	}

	int status = 0;

	std::map<unsigned int, Shader*>::iterator it = m_shaders.find(id);

	Shader *p = new (std::nothrow) Shader();

	if(!p)
		status = 3;

	// If the id is already mapped, release the asset.
	if(it != m_shaders.end() && status != 3) {
		delete (*it).second;
		m_shaders.erase(it);
		status = 2;
	}

	if(p) {
		m_shaders[id] = p;
		
		p->init_shader_programs(ncf->get("vsrc"), ncf->get("fsrc"));
		ShaderManager::get().load_shader_programs(p);
		p->init_default_parameters();
	}

	return status;
}

int AssetManager::create_entity(const NCF1 *ncf)
{
	if(!ncf)
		return 4;

	LogManager::get().log_message("Creating entity [%s]..", ncf->name());

	unsigned int id = to_int(ncf->get("id"));
	
	if (!id) {
		LogManager::get().log_error("Invalid entity id (=0)..");
		return 1;
	}

	int status = 0;

	std::map<unsigned int, Entity*>::iterator it = m_entities.find(id);

	Entity *p = new (std::nothrow) Entity();

	if(!p)
		status = 3;

	// If the id is already mapped, release the asset.
	if(it != m_entities.end() && status != 3) {
		delete (*it).second;
		m_entities.erase(it);
		status = 2;
	}

	if(p) {
		m_entities[id] = p;
	
		p->visibility(to_bool(ncf->get("is_visible")));
		p->state(to_bool(ncf->get("is_active")));
		p->collidable(to_bool(ncf->get("is_collidable")));

		p->shader(to_int(ncf->get("shader")));
		p->material(to_int(ncf->get("material")));
		p->mesh(to_int(ncf->get("mesh")));

		p->texture(TEXENG_TEXTURE_COLORMAP, to_int(ncf->group("texture")->get("color")));
		p->texture(TEXENG_TEXTURE_HEIGHTMAP, to_int(ncf->group("texture")->get("height")));
		p->texture(TEXENG_TEXTURE_NORMALMAP1, to_int(ncf->group("texture")->get("normal1")));
		p->texture(TEXENG_TEXTURE_NORMALMAP2, to_int(ncf->group("texture")->get("normal2")));
		p->texture(TEXENG_TEXTURE_BLENDMAP, to_int(ncf->group("texture")->get("blend")));
	}

	return status;
}

void AssetManager::setup_renderer(IRenderer *renderer)
{
	m_renderer = renderer;
}

void AssetManager::clear()
{
	release();
}

	} /* namespace Core */
} /* namespace TEXEngine */