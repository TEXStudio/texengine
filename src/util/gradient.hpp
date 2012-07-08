#ifndef TEXENGINE_GRADIENT_HPP_INCLUDED
#define TEXENGINE_GRADIENT_HPP_INCLUDED

#include "../math/vector.hpp"
#include "utilenum.h"
#include "color.hpp"
#include "image.hpp"

using TEXEngine::Math::Vector2f;
using TEXEngine::Util::Image;
using TEXEngine::Util::TEXENG_GRADIENT_TYPE;
using TEXEngine::Util::TEXENG_GRADIENT_PERPIXEL;
using TEXEngine::Util::TEXENG_GRADIENT_RADIAL_A;

namespace TEXEngine {
	namespace Util {
		namespace Generator {

// RETURN CODES:
// 0. Everything went well.
// 1. img equals NULL.
// 2. Invalid dimensions.
// 3. Failed to initate img
int gradient(Image *img, const unsigned int width, const unsigned int height,
	const ColorRGBAf &col1, const ColorRGBAf &col2, const Vector2f &v, 
	const bool alpha = 0, const TEXENG_GRADIENT_TYPE type = TEXENG_GRADIENT_RADIAL_A);

int linear_horizontal_gradient(Image *img, const unsigned int width, const unsigned int height,
	const ColorRGBAf &col1, const ColorRGBAf &col2, const Vector2f &start, const Vector2f &end, 
	const bool alpha = false);

		} /* namespace Generator */
	} /* namespace Util */
} /* namespace TEXEngine */

#endif /* TEXENGINE_GRADIENT_HPP_INCLUDED */