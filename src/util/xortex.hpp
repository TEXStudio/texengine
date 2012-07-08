#ifndef TEXENGINE_XORTEX_HPP_INCLUDED
#define TEXENGINE_XORTEX_HPP_INCLUDED

#include "image.hpp"

using TEXEngine::Util::Image;

namespace TEXEngine 
{
	namespace Util 
	{
		namespace Generator 
		{

			// RETURN CODES:
			// 0. Everything went well.
			// 1. img equals NULL.
			// 2. Invalid dimensions.
			// 3. Failed to initate img
			int xortex(Image *img, unsigned int width, unsigned int height);

		} /* namespace Generator */
	} /* namespace Util */
} /* namespace TEXEngine */

#endif /* TEXENGINE_XORTEX_HPP_INCLUDED */