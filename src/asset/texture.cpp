#include "texture.hpp"

namespace TEXEngine {
	namespace Asset {

Texture::Texture()
	: m_type(TEXENG_TEXTURE_COLORMAP),
	  m_id(0)
{}

Texture::Texture(const Texture &tex)
	: m_type(tex.type())
{
	if (&tex == this)
		return;

	m_type = tex.type();
	m_id = 0;
}

Texture &Texture::operator =(const Texture &tex)
{
	if (&tex == this)
		return *this;

	m_type = tex.type();
	m_id = 0;

	return *this;
}

Texture::~Texture()
{}

TEXENG_TEXTURE_TYPE Texture::type() const
{
	return m_type;
}

void Texture::type(TEXENG_TEXTURE_TYPE type)
{
	m_type = type;
}

void Texture::id(GFXAPIuint id)
{
	m_id = id;
}

GFXAPIuint Texture::id() const
{
	return m_id;
}

const Vector2f &Texture::offset()
{
	return m_offset;
}

void Texture::offset(const Vector2f &offset)
{
	m_offset = offset;
}

	} /* namespace Asset */
} /* Namespace TEXEngine */