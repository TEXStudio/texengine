#include "precision.h"
#include "aabb.hpp"

namespace TEXEngine {
	namespace Math {

BoundingBox2::BoundingBox2()
{}

BoundingBox2::BoundingBox2(const Vector2f& a, const Vector2f& b)
{
    min=Vector2f( (a.x<=b.x)? a.x : b.x, (a.y<=b.y)? a.y : b.y );
    max=Vector2f( (a.x>=b.x)? a.x : b.x, (a.y>=b.y)? a.y : b.y );
}

BoundingBox3::BoundingBox3()
{}

BoundingBox3::BoundingBox3(const Vector3f& a, const Vector3f& b)
{
    min=Vector3f( (a.x<=b.x)? a.x : b.x, (a.y<=b.y)? a.y : b.y, (a.z<=b.z)? a.z : b.z );
    max=Vector3f( (a.x>=b.x)? a.x : b.x, (a.y>=b.y)? a.y : b.y, (a.z>=b.z)? a.z : b.z );
}

	} /* namespace Math */
} /* namespace TEXEngine */