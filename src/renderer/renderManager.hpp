#ifndef TEXENGINE_RENDERMANAGER_HPP_INCLUDED
#define TEXENGINE_RENDERMANAGER_HPP_INCLUDED

#include "../message/TEXEngineInterface.hpp"
#include "iRenderer.hpp"

namespace TEXEngine {

	namespace Physics
	{
		class PhysicsDebugRenderer;
	}
	using Physics::PhysicsDebugRenderer;

	namespace Core {

class RenderManager : public Object
{
	//physics debug renderer need access to renderer
	friend class PhysicsDebugRenderer;

	public:
		static RenderManager& get();

		void on_event(const unsigned long event_code, const Message& parameters);
		void setup();

		void viewport(unsigned int w = 800, unsigned int h = 600);

		const ColorRGBAf clear_color();
		void clear_color(const ColorRGBAf &color);

		void render_scene();
		void submit_frame();
					
	private:
		RenderManager();
		~RenderManager();

		void change_update_rate(const unsigned long update_rate);
		
		// RETURN CODES:
		// 0. Renderer was initiated.
		// 1. Renderer has already been initiated. Action supressed.
		// 2. Failed to initiate the renderer.
	#if defined(_WIN32)
		int init(HWND handle);

	#else
		int init();

	#endif /* _WIN32 */

		// RETURN CODES:
		// 0. Renderer was initiated.
		// 1. Renderer is not initialized.
		int deinit();

		unsigned long m_update_rate;
		static RenderManager m_RenderManager;
	
	public: // TEMPORARILY PUBLIC, UNTIL SCENEGRAPH IS IMPLEMENTED.
		IRenderer* m_renderer;
};

	} /* namespace Core */
} /* namespace TEXEngine */

#endif /* TEXENGINE_RENDERMANAGER_HPP_INCLUDED */