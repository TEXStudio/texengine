#ifndef TEXENGINE_COLOR_HPP_INCLUDED
#define TEXENGINE_COLOR_HPP_INCLUDED

namespace TEXEngine {
	namespace Util {

// Forward declarations
class ColorRGBf;
class ColorRGBAf;

/*
    COLOR RGB
*/
class ColorRGBf
{
	public:
		explicit ColorRGBf(float r = 1.0f, float g = 1.0f, float b = 1.0f);
		explicit ColorRGBf(const ColorRGBAf &color);

		// Operators
		inline ColorRGBf &operator +=(const ColorRGBf &c);
		inline ColorRGBf &operator -=(const ColorRGBf &c);
		inline ColorRGBf &operator *=(float r);

		float red, green, blue;

};

// Auxiliary operators
inline const ColorRGBf operator +(const ColorRGBf &c1, const ColorRGBf &c2);
inline const ColorRGBf operator -(const ColorRGBf &c1, const ColorRGBf &c2);
inline const ColorRGBf operator *(const ColorRGBf &c, float r);

/*
    COLOR RGBA
*/
class ColorRGBAf
{
	public:
		explicit ColorRGBAf(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);
		explicit ColorRGBAf(const ColorRGBf &color);

		// Operators
		inline ColorRGBAf &operator +=(const ColorRGBAf &c);
		inline ColorRGBAf &operator -=(const ColorRGBAf &c);
		inline ColorRGBAf &operator *=(float r);

		float red, green, blue, alpha;
};

// Auxiliary operators
inline const ColorRGBAf operator +(const ColorRGBAf &c1, const ColorRGBAf &c2);
inline const ColorRGBAf operator -(const ColorRGBAf &c1, const ColorRGBAf &c2);
inline const ColorRGBAf operator *(const ColorRGBAf &c, float r);

	}/* namespace Util */
} /* Namespace TEXEngine */

#include "color.inl"

#endif /* TEXENGINE_COLOR_HPP_INCLUDED */