#if defined(_WIN32)
	#include "../port/pcRenderer.hpp"
	using TEXEngine::Port::PCRenderer;
#endif /* _WIN32 */

#include "../math/matrix.hpp"
#include "../math/vector.hpp"
#include "../physics/PhysicsManager.hpp"
#include "../asset/assetEnum.h"
#include "../log/logmanager.hpp"
#include "../ui/uiManager.hpp"
#include "../asset/AssetManager.hpp"
#include "../asset/camera.hpp"
#include "../scene/scene.hpp"
#include "../scene/SceneManager.hpp"
#include "renderManager.hpp"

using TEXEngine::Math::Matrix4x4f;
using TEXEngine::Math::Vector2f;
using TEXEngine::Asset::TEXENG_TEXTURE_COLORMAP;
using TEXEngine::Asset::TEXENG_TEXTURE_NORMALMAP1;
using TEXEngine::Asset::TEXENG_TEXTURE_NORMALMAP2;
using TEXEngine::Asset::TEXENG_TEXTURE_HEIGHTMAP;
using TEXEngine::Asset::TEXENG_TEXTURE_BLENDMAP;
using TEXEngine::Asset::Camera;
using TEXEngine::Core::PhysicsManager;
using TEXEngine::Core::SceneManager;

namespace TEXEngine {
	namespace Core {

RenderManager RenderManager::m_RenderManager;

RenderManager::RenderManager() :
	Object(1) , m_update_rate(UPDATE) , m_renderer(NULL)
{}

RenderManager::~RenderManager()
{
	deinit();
}

#if defined(_WIN32)
int RenderManager::init(HWND handle)
#else
int RenderManager::init()
#endif /* _WIN32 */
{
	LogManager::get().log_message("Attempting to create and initiate the renderer..");

	if (m_renderer) {
		LogManager::get().log_warning("The renderer has already been initiated.");
		return 1;
	}

	#if defined(_WIN32)
		m_renderer = new (std::nothrow) PCRenderer();
	#endif /* _WIN32 */

	if (!m_renderer) {
		LogManager::get().log_error("Failed to create the renderer.");
		return 2;
	}

	LogManager::get().log_message("Created the renderer.");

	int status = 0;

	#if defined(_WIN32)
		status = m_renderer->init(handle);

	#else
		status = m_renderer->init();

	#endif /* _WIN32 */

	if (!status) {
		LogManager::get().log_message("Initiated the renderer.");
		UIManager::get().setup_renderer(m_renderer);
		AssetManager::get().setup_renderer(m_renderer);

		// Just clear the buffer.
		m_renderer->scene_begin();
		m_renderer->scene_end();

	} else {
		LogManager::get().log_error("Failed to initiate the renderer.");
	}

	return status;
}

int RenderManager::deinit()
{
	if(!m_renderer) {
		return 1;
	}
	
	m_renderer->deinit();
	delete m_renderer;
	m_renderer = NULL;

	return 0;
}

RenderManager& RenderManager::get()
{
	return m_RenderManager;
}

void RenderManager::viewport(unsigned int w, unsigned int h)
{
	#if defined(_WIN32)
		if (!m_renderer)
			return;

		m_renderer->viewport_width(w);
		m_renderer->viewport_height(h);

	#endif /* _WIN32 */
}

const ColorRGBAf RenderManager::clear_color()
{
	if(m_renderer)
		return m_renderer->clear_color();

	return ColorRGBAf(0, 0, 0, 1);
}


void RenderManager::clear_color(const ColorRGBAf &color)
{
	if(m_renderer)
		m_renderer->clear_color(color);
}

void RenderManager::render_scene()
{
	if (!m_renderer)
		return;

	m_renderer->scene_begin();

	// Get a pointer to the active scene.
	Scene *scene = SceneManager::get().active();

	// Check for NULL
	if(!scene)
		return;

	// Here it just parses the entity list and calls renderer functions.
	const std::vector<unsigned int> &entities = scene->entities();
	std::vector<unsigned int>::const_iterator it;

	for (it = scene->entities().begin(); it != scene->entities().end(); it++) {
		Entity *entity = AssetManager::get().get_entity((*it));
		if (entity) {
			if(entity->is_active() && entity->is_visible()) {
				// Activate shader.
				Shader *shader = AssetManager::get().get_shader(entity->shader());

				if(shader) {
					shader->bind_fragment_program();
				
					// Pass material to shader.
					Material *material = AssetManager::get().get_material(entity->material());
					if (material) {
						shader->update_material(material);
					}

					// pass textures to shader
					Texture *tex_blend = AssetManager::get().get_texture(entity->texture(TEXENG_TEXTURE_BLENDMAP));
					Texture *tex_normal1 = AssetManager::get().get_texture(entity->texture(TEXENG_TEXTURE_NORMALMAP1));
					Texture *tex_normal2 = AssetManager::get().get_texture(entity->texture(TEXENG_TEXTURE_NORMALMAP2));
					Texture *tex_height = AssetManager::get().get_texture(entity->texture(TEXENG_TEXTURE_HEIGHTMAP));
					Texture *tex_colors = AssetManager::get().get_texture(entity->texture(TEXENG_TEXTURE_COLORMAP));			
					
					if(tex_blend) {
						m_renderer->activate_texture(tex_blend);
						
						Vector2f tc = tex_blend->offset();
						tc += Vector2f(0.001f, 0.001f);
						tc = Vector2f(tc.x > 1.f ? 0.0 : tc.x, tc.y > 1.f ? 0.0 : tc.y);
						tex_blend->offset(tc);
						
						shader->update_blend_map_texture(tex_blend);
					}

					if(tex_normal1) {
						m_renderer->activate_texture(tex_normal1);

						Vector2f tc = tex_normal1->offset();
						tc += Vector2f(0.001f, 0.001f);
						//tc = Vector2f(tc.x > 1.f ? 0.0 : tc.x, tc.y > 1.f ? 0.0 : tc.y);
						tex_normal1->offset(tc);
						
						shader->update_normal_map_texture(tex_normal1);
					}

					if(tex_normal2) {
						m_renderer->activate_texture(tex_normal2);

						Vector2f tc = tex_normal2->offset();
						tc += Vector2f(0.001f, 0.001f);
						//tc = Vector2f(tc.x > 1.f ? 0.0 : tc.x, tc.y > 1.f ? 0.0 : tc.y);
						tex_normal2->offset(tc);
						
						shader->update_normal_map_two_texture(tex_normal2);
					}

					if(tex_height) {
						m_renderer->activate_texture(tex_height);
						
						Vector2f tc = tex_height->offset();
						tc += Vector2f(0.001f, 0.001f);
					//	tc = Vector2f(tc.x > 1.f ? 0.0 : tc.x, tc.y > 1.f ? 0.0 : tc.y);
						tex_height->offset(tc);
						
						shader->update_height_map_texture(tex_height);
					}
				
					if(tex_colors) {
						m_renderer->activate_texture(tex_colors);
						
						Vector2f tc = tex_colors->offset();
						tc += Vector2f(0.001f, 0.001f);
						//
						tc = Vector2f(tc.x > 1.f ? 0.0 : tc.x, tc.y > 1.f ? 0.0 : tc.y);
						tex_colors->offset(tc);
						
						shader->update_color_texture(tex_colors);
					}
					
					// pass lights to shader
					const std::map<unsigned int, Light*> &lights = AssetManager::get().lights();
					std::map<unsigned int, Light*>::const_iterator lt;
					unsigned int lightn = 0;
					for (lt = lights.begin(); lt != lights.end(); lt++) {
						Light *light = (*lt).second;
						if (light) {
							if(light->is_active()) {
								switch (lightn) {
									case 0:
										shader->update_light_one(light);
										break;
									case 1:
										shader->update_light_two(light);
										break;
									case 2:
										shader->update_light_three(light);
										break;
									case 3:
										shader->update_light_four(light);
										break;
								}


								// We are limiting the number of lights to 4 to lower the complexity
								// of the shader parameter passing mechanism.
								// Only the first 4 lights in the list will be considered.
								if(++lightn == 4)
									break;
							}
						}
					}

					shader->fragment_program_paramter_applying_complete();

					shader->bind_vertex_program();
					
					// Use active camera.
					Camera *cam = AssetManager::get().get_camera((unsigned int) scene->active_camera());
					
					if (cam)
					{
						// Ton pairnete kirie, prepei na to baleis ston renderer
						glMatrixMode(GL_MODELVIEW);
						glLoadIdentity();
						//Apply/update model view matrix
						cam->render();
						shader->update_camera(cam);
					}

					Matrix4x4f final_mat = entity->matrix().transposed();
					
					if (entity->physics_obj()) {
						final_mat *= entity->physics_obj()->get_transform_matrix();
					}

					shader->update_transform_matrix(final_mat);
					shader->update_model_view_projection_matrix();
					shader->update_projection_matrix();

					shader->vertex_program_parameter_applying_complete();

					shader->bind_programs();
				}

				// Render mesh
				Mesh *mesh = AssetManager::get().get_mesh(entity->mesh());
				if (mesh)
					m_renderer->render_mesh(mesh);

				if (shader)
					shader->unbind_programs();
		
				m_renderer->deactivate_texture();
			}
		}
	}

	//Physics debug rendering
	if(PhysicsManager::get().get_enable_debug_render())
		PhysicsManager::get().debug_render();
}

void RenderManager::submit_frame()
{
	m_renderer->scene_end();
}

void RenderManager::change_update_rate(const unsigned long update_rate)
{
	if (update_rate != m_update_rate) {
		if (update_rate >= UPDATE  &&  update_rate <= UPDATE_SLOW) {
			unregister_event(m_update_rate);
			m_update_rate = update_rate;
			register_event(m_update_rate);
		}
	}
}

void RenderManager::on_event(const unsigned long event_code, const Message& parameters)
{
	/*if (event_code == m_update_rate) {
		// do update stuff here
	}
	else*/ if (event_code == INITIALISE){
	#if defined(_WIN32)
		HWND handle = (HWND)parameters.get_pointer(0);

		if ( handle )
			init(handle);
		else
			LogManager::get().log_error("Window handle parameter invalid! Render initialisation failed.");
	#else
		init();
	#endif
	}
	/*else if (event_code == EXIT) {
		if ( deinit() )
			// Throw error log here
	}
	else if (event_code == RENDER_MANAGER_CHANGE_UPDATE_RATE) {
		change_update_rate((unsigned long)parameters.get_integral_data(2));
	}*/
}

void RenderManager::setup()
{
	register_event(INITIALISE);
	//register_event(EXIT);
	//register_event(RENDER_MANAGER_CHANGE_UPDATE_RATE);
	//register_event(m_update_rate);
}

	} /* namespace Core */
} /* namespace TEXEngine */