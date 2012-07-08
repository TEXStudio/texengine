#ifndef TEXENGINE_AABB_HPP_INCLUDED
#define TEXENGINE_AABB_HPP_INCLUDED

#include "vector.hpp"

namespace TEXEngine {
	namespace Math {

class BoundingBox2
{
    public:
        BoundingBox2();
        BoundingBox2(const Vector2f& a, const Vector2f& b);

        inline bool contains(const Vector2f& p) const;          // returns true if the given point is within the bounds of the box, else false
        inline Vector2f center() const;                         // returns the center coordinates of the box

        inline void augment(const Vector2f& v);                 // augments the bounding box to include the given vector
        inline void augment(const BoundingBox2& b);             // augments the bounding box to include the given bounding box

        Vector2f min, max;
};

class BoundingBox3
{
    public:
        BoundingBox3();
        BoundingBox3(const Vector3f& a, const Vector3f& b);

        inline bool contains(const Vector3f& p) const;          // returns true if the given point is within the bounds of the box, else false
        inline Vector3f center() const;                         // returns the center coordinates of the box

        inline void augment(const Vector3f& v);                 // augments the bounding box to include the given vector
        inline void augment(const BoundingBox3& b);             // augments the bounding box to include the given bounding box

        Vector3f min, max;
};

	} /* namespace Math */
} /* namespace TEXEngine */

#include "aabb.inl"

#endif /* TEXENGINE_AABB_HPP_INCLUDED */