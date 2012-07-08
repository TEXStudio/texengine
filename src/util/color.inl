#ifndef TEXENGINE_COLOR_INL_INCLUDED
#define TEXENGINE_COLOR_INL_INCLUDED

#ifndef TEXENGINE_COLOR_HPP_INCLUDED
    #error "color.hpp must be included before color.inl"
#endif /* TEXENGINE_COLOR_HPP_INCLUDED */

#include "../math/precision.h"

namespace TEXEngine {
	namespace Util {

// ColorRGBf
inline ColorRGBf &ColorRGBf::operator +=(const ColorRGBf& c)
{
	red += c.red;
	green += c.green;
	blue += c.blue;

	return *this;
}

inline ColorRGBf &ColorRGBf::operator -=(const ColorRGBf& c)
{
	red -= c.red;
	green -= c.green;
	blue -= c.blue;

	return *this;
}

inline ColorRGBf &ColorRGBf::operator *=(float r)
{
	red *= r;
	green *= r;
	blue *= r;

	return *this;
}

inline const ColorRGBf operator +(const ColorRGBf& c1, const ColorRGBf& c2)
{
	return ColorRGBf(c1) += c2;
}

inline const ColorRGBf operator -(const ColorRGBf& c1, const ColorRGBf& c2)
{
	return ColorRGBf(c1) -= c2;
}

inline const ColorRGBf operator *(const ColorRGBf& c, float r)
{
	return ColorRGBf(c) *= r;
}

// ColorRGBAf
inline ColorRGBAf &ColorRGBAf::operator +=(const ColorRGBAf& c)
{
	red += c.red;
	green += c.green;
	blue += c.blue;
	alpha += c.alpha;

	return *this;
}

inline ColorRGBAf &ColorRGBAf::operator -=(const ColorRGBAf& c)
{
	red -= c.red;
	green -= c.green;
	blue -= c.blue;
	alpha -= c.alpha;

	return *this;
}

inline ColorRGBAf &ColorRGBAf::operator *=(float r)
{
	red *= r;
	green *= r;
	blue *= r;
	alpha *= r;

	return *this;
}

inline const ColorRGBAf operator +(const ColorRGBAf& c1, const ColorRGBAf& c2)
{
	return ColorRGBAf(c1) += c2;
}

inline const ColorRGBAf operator -(const ColorRGBAf& c1, const ColorRGBAf& c2)
{
	return ColorRGBAf(c1) -= c2;
}

inline const ColorRGBAf operator *(const ColorRGBAf& c, float r)
{
	return ColorRGBAf(c) *= r;
}

	}/* namespace Util */
} /* Namespace TEXEngine */

#endif /* TEXENGINE_COLOR_INL_INCLUDED */