#ifndef TEXENGINE_IMAGE_HPP_INCLUDED
#define TEXENGINE_IMAGE_HPP_INCLUDED

#include "../util/buffer.hpp"
#include "../util/color.hpp"

namespace TEXEngine {
	namespace Util {

class Image
{
	public:
		Image();
		Image(const Image &img);
		Image &operator =(const Image &img);
		~Image();

		// NOTES:
		// If the user sets one or both of the dimensions as 0
		// it is equivalent to resetting the buffer as total size is 0.
		// RETURN CODES:
		//	0. Everything went well.
		//  1. Failed to initialize pixels.
		int init(unsigned int w, unsigned int h);

		unsigned int width() const;
		unsigned int height() const;

		const Buffer<ColorRGBAf> &pixels() const;
		Buffer<ColorRGBAf> &pixels_w();

		const ColorRGBAf &pixel(unsigned int x, unsigned int y) const;
		ColorRGBAf &pixel_w(unsigned int x, unsigned int y);

	private:
		unsigned int m_width;
		unsigned int m_height;
		Buffer<ColorRGBAf> m_pixels;
};

	} /* namespace Util */
} /* Namespace TEXEngine */

#endif /* TEXENGINE_TEXTURE_HPP_INCLUDED */