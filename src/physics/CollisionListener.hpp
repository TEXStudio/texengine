#ifndef TEXENGINE_COLLISIONLISTENER_HPP_INCLUDED
#define TEXENGINE_COLLISIONLISTENER_HPP_INCLUDED

//#include "PhysicsStructs.h"

namespace TEXEngine {
	namespace Physics {

class CollisionListener
{
public:
	virtual ~CollisionListener(void) {};

	virtual void react_to(unsigned int entityID) = 0;
};

	} /* namespace Physics */
} /* namespace TEXEngine */

#endif /* TEXENGINE_COLLISIONLISTENER_HPP_INCLUDED */
