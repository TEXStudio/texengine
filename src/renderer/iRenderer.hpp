#ifndef TEXENGINE_IRENDERER_HPP_INCLUDED
#define TEXENGINE_IRENDERER_HPP_INCLUDED

#if defined(_WIN32)
	// Needed for the definition of HWND
	#define WIN32_LEAN_AN_MEAN
	#define NOMINMAX
#include <Windows.h>
#endif /* _WIN32 */

#include "../math/matrix.hpp"
#include "../util/utilenum.h"
#include "../asset/light.hpp"
#include "../asset/camera.hpp"
#include "../asset/mesh.hpp"
#include "../asset/texture.hpp"
#include "../asset/material.hpp"
#include "../asset/shader.hpp"
#include "../util/color.hpp"

using TEXEngine::Util::ColorRGBAf;
using TEXEngine::Math::Matrix4x4f;
using TEXEngine::Asset::Light;
using TEXEngine::Asset::Camera;
using TEXEngine::Asset::Mesh;
using TEXEngine::Asset::Texture;
using TEXEngine::Asset::Material;
using TEXEngine::Asset::Shader;
using TEXEngine::Util::TEXENG_RENDERING_TYPE;

namespace TEXEngine {

	//Physics debug render needs acces to the drawline function
	namespace Physics
	{
		class PhysicsDebugRenderer;
	}
	using Physics::PhysicsDebugRenderer;

	namespace Core {

class IRenderer
{
	friend class UIManager;
	friend class AssetManager;
	friend class RenderManager;
	friend class PhysicsDebugRenderer; //Physics debug render needs acces to the drawline function
	protected:
		IRenderer();
		virtual ~IRenderer();
			
		// RETURN CODES:
		// 0. Everything went well.
		// 1. Failed to initialize the device.
	#if defined(_WIN32)
		virtual int init(HWND handle) = 0;

	#else
		virtual int init() = 0;

	#endif /* _WIN32 */

		// RETURN CODES:
		// 0. Everything went well.
		// 1. Failed to shut down the device.
		virtual int deinit() = 0;

		virtual void scene_begin() = 0;
		virtual void scene_end() = 0;
		virtual void set_rendering_mode(TEXENG_RENDERING_TYPE mode) = 0;

		// Matrix operations
		virtual void set_matrix_mode(TEXENG_MATRIX_TYPE mode) = 0;
		virtual void set_matrix(const Matrix4x4f *matrix) = 0;
		virtual void set_matrix_identity() = 0;
		virtual void multiply_matrix(const Matrix4x4f &matrix) = 0;
		virtual void push_matrix() = 0;		
		virtual void pop_matrix() = 0;
		
		// Generic drawers
		virtual void draw_line(const Vector3f &from, const Vector3f &to, const ColorRGBAf &color) = 0;

		// Asset handling.
		virtual void initiate_mesh(Mesh *mesh) = 0;
		virtual void deinitiate_mesh(Mesh *mesh) = 0;
		virtual void render_mesh(Mesh *mesh) = 0;

		virtual void initiate_texture(Texture *texture) = 0;
		virtual void deinitiate_texture(Texture *texture) = 0;
		virtual void activate_texture(Texture *texture) = 0;
		virtual void deactivate_texture() = 0;

		// Callbacks.
		virtual void render_box(float x, float y, float w, float h, 
			const ColorRGBAf &col, int texid = 0,
			const Vector2f &uv1 = Vector2f(0,0), const Vector2f &uv2 = Vector2f(0,0), 
			const Vector2f &uv3 = Vector2f(0,0), const Vector2f &uv4 = Vector2f(0,0)) = 0;
		
		// Non-virtual functions
		Vector2f map_2d(const int x, const int y);

		float aspect_modifier_2d();
		void aspect_modifier_2d(float mod);

		const ColorRGBAf &clear_color();
		void clear_color(const ColorRGBAf &color);

		unsigned int viewport_width();
		unsigned int viewport_height();
		void viewport_width(const unsigned int iViewportWidth);
		void viewport_height(const unsigned int iViewportHeight);

		void matrix_to_1D_array(float* outArray, const Matrix4x4f& matrix);
		void matrix_to_1D_array(float* outArray, const Matrix4x4f* matrix);

	private:
		float m_aspect_modifier_2d;
		unsigned int m_iViewportWidth;
		unsigned int m_iViewportHeight;
		ColorRGBAf m_clear_color;

};

	} /* namespace Core */
} /* namespace TEXEngine */

#endif /* TEXENGINE_IRENDERER_HPP_INCLUDED */