#ifndef TEXENGINE_PHYSICSDEBUGRENDERER_HPP_INCLUDED
#define TEXENGINE_PHYSICSDEBUGRENDERER_HPP_INCLUDED

/**Note that this class inherits from a bullet intefrace class***/

#include "LinearMath/btIDebugDraw.h"

namespace TEXEngine
{
	namespace Core
	{
		class PhysicsManager;
		class RenderManager;
	}

	using Core::PhysicsManager;
	using Core::RenderManager;

	namespace Physics 
	{

		class PhysicsDebugRenderer : public btIDebugDraw
		{
			friend class PhysicsManager;
			friend class RenderManager;

		protected:
			PhysicsDebugRenderer();
			void setDebugMode(int debugMode);
			int	 getDebugMode() const;
			//Debug rendering
			void drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor);
			void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
			void drawSphere (const btVector3& p, btScalar radius, const btVector3& color);
			void drawBox (const btVector3& boxMin, const btVector3& boxMax, const btVector3& color, btScalar alpha);
			void drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha);
			void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
			void reportErrorWarning(const char* warningString);
			void draw3dText(const btVector3& location,const char* textString);

		private:
			int m_debugMode;

		};
	}
}

#endif //TEXENGINE_PHYSICSDEBUGRENDERER_HPP_INCLUDED