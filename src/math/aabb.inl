#ifndef TEXENGINE_AABB_INL_INCLUDED
#define TEXENGINE_AABB_INL_INCLUDED

#ifndef TEXENGINE_AABB_HPP_INCLUDED
    #error "aabb.hpp must be included before aabb.inl"
#endif /* TEXENGINE_AABB_H_INCLUDED */

namespace TEXEngine {
	namespace Math {

inline bool BoundingBox2::contains(const Vector2f& p) const
{
    return (p.x >= min.x) && (p.y>=min.y) && (p.x<=max.x) && (p.y<=max.y);
}

inline Vector2f BoundingBox2::center() const
{
    return (min + max) / 2;
}

inline void BoundingBox2::augment(const Vector2f& v)
{
    if(v.x > max.x)	max.x = v.x;
    if(v.x < min.x)	min.x = v.x;

    if(v.y > max.y)	max.y = v.y;
    if(v.y < min.y)	min.y = v.y;
}

inline void BoundingBox2::augment(const BoundingBox2& b)
{
    if(b.max.x > max.x)	max.x = b.max.x;
    if(b.min.x < min.x)	min.x = b.min.x;

    if(b.max.y > max.y)	max.y = b.max.y;
    if(b.min.y < min.y)	min.y = b.min.y;
}

inline bool BoundingBox3::contains(const Vector3f& p) const
{
    return (p.x>= min.x) && (p.y>=min.y) && (p.z>=min.z) && (p.x<=max.x) && (p.y<=max.y) && (p.z<=max.z);
}

inline Vector3f BoundingBox3::center() const
{
    return (min + max) / 2;
}

inline void BoundingBox3::augment(const Vector3f& v)
{
    if(v.x > max.x)	max.x = v.x;
    if(v.x < min.x)	min.x = v.x;

    if(v.y > max.y)	max.y = v.y;
    if(v.y < min.y)	min.y = v.y;

    if(v.z > max.z)	max.z = v.z;
    if(v.z < min.z)	min.z = v.z;
}

inline void BoundingBox3::augment(const BoundingBox3& b)
{
    if(b.max.x > max.x)	max.x = b.max.x;
    if(b.min.x < min.x)	min.x = b.min.x;

    if(b.max.y > max.y)	max.y = b.max.y;
    if(b.min.y < min.y)	min.y = b.min.y;

    if(b.max.z > max.z)	max.z = b.max.z;
    if(b.min.z < min.z)	min.z = b.min.z;
}

	} /* namespace Math */
} /* namespace TEXEngine */

#endif /* TEXENGINE_AABB_INL_INCLUDED */
