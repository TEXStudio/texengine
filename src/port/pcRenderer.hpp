#ifndef TEXENGINE_PCRENDERER_HPP_INCLUDED
#define TEXENGINE_PCRENDERER_HPP_INCLUDED

#if defined(_WIN32)

#include <vector>
#include "../port/gfxapi.h"
#include "../renderer/irenderer.hpp"

using TEXEngine::Core::IRenderer;

namespace TEXEngine {

	using namespace Core;
	namespace Port {

class PCRenderer : public IRenderer
{
	friend class RenderManager;
	protected:
		PCRenderer();
		~PCRenderer();

		int init(HWND param);
		int deinit();
		
		void scene_begin();
		void scene_end();
		void set_rendering_mode(TEXENG_RENDERING_TYPE mode);

		void set_matrix_mode(TEXENG_MATRIX_TYPE mode);
		void set_matrix(const Matrix4x4f *matrix);
		void set_matrix_identity();
		void multiply_matrix(const Matrix4x4f &matrix);
		void push_matrix();		
		void pop_matrix();

		void draw_line(const Vector3f &from, const Vector3f &to, const ColorRGBAf &color);

		void initiate_mesh(Mesh *mesh);
		void deinitiate_mesh(Mesh *mesh);
		void render_mesh(Mesh *mesh);

		void initiate_texture(Texture *texture);
		void deinitiate_texture(Texture *texture);
		void activate_texture(Texture *texture);
		void deactivate_texture();

		void render_box(float x, float y, float w, float h, 
			const ColorRGBAf &col, int texid = 0,
			const Vector2f &uv1 = Vector2f(0,0), const Vector2f &uv2 = Vector2f(0,0), 
			const Vector2f &uv3 = Vector2f(0,0), const Vector2f &uv4 = Vector2f(0,0));

	private:
		PIXELFORMATDESCRIPTOR m_pfd;
		GFXAPIint m_pixel_format;
		HDC m_hdc;
		HGLRC m_hrc;
		HWND m_hwnd;

		std::vector<GFXAPIuint> m_buffer_ids;
		std::vector<GFXAPIuint> m_texture_ids;
};

	} /* namespace Port */
} /* namespace TEXEngine */

#endif /* _WIN32 */

#endif /* TEXENGINE_PCRENDERER_HPP_INCLUDED */