#include "iRenderer.hpp"

namespace TEXEngine {
	namespace Core {

IRenderer::IRenderer()
	: m_iViewportWidth(0),
	  m_iViewportHeight(0),
	  m_aspect_modifier_2d(1.f),
	  m_clear_color(ColorRGBAf(0, 0, 0, 1))
{}

IRenderer::~IRenderer()
{}

Vector2f IRenderer::map_2d(const int x, const int y)
{	
	float aspect = (float)viewport_width() / (float)viewport_height();
	return Vector2f(((float)x / (float)viewport_width()  * 2.0f - 1.0f) * aspect / aspect_modifier_2d(), 
			-((float)y / (float)viewport_height()  * 2.0f - 1.0f));
}

float IRenderer::aspect_modifier_2d()
{
	return m_aspect_modifier_2d;
}

void IRenderer::aspect_modifier_2d(float mod)
{
	m_aspect_modifier_2d = mod;
}


const ColorRGBAf &IRenderer::clear_color()
{
	return m_clear_color;
}

void IRenderer::clear_color(const ColorRGBAf &color)
{
	m_clear_color = color;
}

unsigned int IRenderer::viewport_width()
{
	return m_iViewportWidth;
}

unsigned int IRenderer::viewport_height() 
{
	return m_iViewportHeight;
}

void IRenderer::viewport_width(const unsigned int iViewportWidth)
{
	m_iViewportWidth = iViewportWidth;
}

void IRenderer::viewport_height(const unsigned int iViewportHeight)
{
	m_iViewportHeight = iViewportHeight;
}

void IRenderer::matrix_to_1D_array(float* outArray, const Matrix4x4f* matrix)
{
	Matrix4x4f mat = *matrix;
	for(int i = 0; i < 16; i++)
	{
		outArray[i] = (float)*mat[i];
	}
}
void IRenderer::matrix_to_1D_array(float* outArray, const Matrix4x4f& matrix)
{
	Matrix4x4f mat = matrix;
	for(int i = 0; i < 16; i++)
	{
		outArray[i] = (float)*mat[i];
	}
}

	} /* namespace Core */
} /* namespace TEXEngine */