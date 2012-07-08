#ifndef TEXENGINE_PPM_HPP_INCLUDED
#define TEXENGINE_PPM_HPP_INCLUDED

#include "image.hpp"

using TEXEngine::Util::Image;

namespace TEXEngine {
	namespace Util {
		namespace IO {
			namespace Import {

//	RESTRICTIONS:
//	The loader does not support comments. If any comments
//	exist in your file (lines starting with #) you should
//	remove them manually.
//
//	Also only images with a width per pixel component equal
//  to 255 are supported. This is the most common format.
//
// RETURN CODES:
// 0. Everything went well.
// 1. filename equals NULL.
// 2. img equals NULL.
// 3. Failed to open file.
// 4. Invalid format.
// 5. Corrupted file.
// 6. Failed to initialize the Image.
int ppm_raw(const char *filename, Image *img);

			} /* namespace Import */

			namespace Export {

// RETURN CODES:
// 0. Everything went well.
// 1. filename equals NULL.
// 2. img equals NULL.
// 3. Nothing to export (an img dimension is equal to 0).
// 4. Failed to open file.
int ppm_raw(const char *filename, Image *img);

			} /* namespace Export */

		} /* namespace IO */
	} /* namespace Util */
} /* namespace TEXEngine */

#endif /* TEXENGINE_PPM_HPP_INCLUDED */