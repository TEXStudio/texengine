
#include "physicsDebugRenderer.hpp"
#include "../renderer/renderManager.hpp"
#include "../util/color.hpp"
#include "../math/vector.hpp"

//We only need the line rendering
//function, therefore the rest
//of the functions are left empty
using TEXEngine::Math::Vector3f;
using TEXEngine::Util::ColorRGBAf;

namespace TEXEngine
{
	namespace Physics 
	{
		PhysicsDebugRenderer::PhysicsDebugRenderer()
		{

		}
		void PhysicsDebugRenderer::setDebugMode(int debugMode)
		{
			m_debugMode = debugMode;
		}
		int	 PhysicsDebugRenderer::getDebugMode() const
		{
			return m_debugMode;
		}

		//Debug rendering
		void PhysicsDebugRenderer::drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor)
		{
			Vector3f vfrom = Vector3f(from.getX(), from.getY(), from.getZ());
			Vector3f vto = Vector3f(to.getX(), to.getY(), to.getZ());
			ColorRGBAf color = ColorRGBAf(1.0f, 0.0f, 0.0f, 1.0f);

			RenderManager::get().m_renderer->draw_line(vfrom, vto, color);
		}
		void PhysicsDebugRenderer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
		{
			drawLine(from, to, color, color);
		}
		void PhysicsDebugRenderer::drawSphere(const btVector3& p, btScalar radius, const btVector3& color)
		{

		}
		void PhysicsDebugRenderer::drawBox(const btVector3& boxMin, const btVector3& boxMax, const btVector3& color, btScalar alpha)
		{

		}
		void PhysicsDebugRenderer::drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha)
		{

		}
		void PhysicsDebugRenderer::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
		{

		}
		void PhysicsDebugRenderer::reportErrorWarning(const char* warningString)
		{

		}
		void PhysicsDebugRenderer::draw3dText(const btVector3& location,const char* textString)
		{

		}
	}
}