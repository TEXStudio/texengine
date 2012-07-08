#include "color.hpp"

namespace TEXEngine {
	namespace Util {

// ColorRGBf
ColorRGBf::ColorRGBf(float r, float g, float b)
	: red(r), green(g), blue(b)
{}

ColorRGBf::ColorRGBf(const ColorRGBAf &color)
	: red(color.red), green(color.green), blue(color.blue)
{}

// ColorRGBAf
ColorRGBAf::ColorRGBAf(float r, float g, float b, float a)
	: red(r), green(g), blue(b), alpha(a)
{}

ColorRGBAf::ColorRGBAf(const ColorRGBf &color)
	: red(color.red), green(color.green), blue(color.blue), alpha(1.0f)
{}

	}/* namespace Util */
} /* Namespace TEXEngine */
