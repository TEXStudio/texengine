#include "image.hpp"

namespace TEXEngine {
	namespace Util {

Image::Image()
	: m_width(0), m_height(0)
{}

Image::Image(const Image &img)
{
	if (&img == this)
		return;

	m_pixels = img.pixels();
	m_width = img.width();
	m_height = img.height();
}

Image &Image::operator =(const Image &img)
{
	if (&img == this)
		return *this;

	m_pixels = img.pixels();
	m_width = img.width();
	m_height = img.height();

	return *this;
}

Image::~Image()
{}

unsigned int Image::width() const
{
	return m_width;
}

unsigned int Image::height() const
{
	return m_height;
}

const Buffer<ColorRGBAf> &Image::pixels() const
{
	return m_pixels;
}

Buffer<ColorRGBAf> &Image::pixels_w()
{
	return m_pixels;
}

const ColorRGBAf &Image::pixel(unsigned int x, unsigned int y) const
{
	int nx = x >= m_width ? m_width - 1 : x;
	int ny = y >= m_height ? m_height - 1 : y;

	return m_pixels.data()[ny * m_width + nx];
}

ColorRGBAf &Image::pixel_w(unsigned int x, unsigned int y)
{
	int nx = x >= m_width ? m_width - 1 : x;
	int ny = y >= m_height ? m_height - 1 : y;

	return m_pixels.data()[ny * m_width + nx];
}

int Image::init(unsigned int w, unsigned int h)
{
	if(m_pixels.init(w*h))
		return 1;

	m_width = w;
	m_height = h;

	return 0;
}

	} /* namespace Util */
} /* Namespace TEXEngine */