#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <GL\glew.h>
#include <GL\gl.h>
#include <GL\glu.h>

#include "pcRenderer.hpp"
#include "../log/logmanager.hpp"
#include "../asset/assetManager.hpp"

using TEXEngine::Port::vertex_t;
using TEXEngine::Port::index_t;

using TEXEngine::Asset::TEXENG_TEXTURE_TYPE;
using TEXEngine::Asset::TEXENG_TEXTURE_GENERIC;
using TEXEngine::Asset::TEXENG_TEXTURE_SPRITE;
using TEXEngine::Asset::TEXENG_TEXTURE_FONT;
using TEXEngine::Asset::TEXENG_TEXTURE_COLORMAP;
using TEXEngine::Asset::TEXENG_TEXTURE_HEIGHTMAP;
using TEXEngine::Asset::TEXENG_TEXTURE_NORMALMAP1;
using TEXEngine::Asset::TEXENG_TEXTURE_NORMALMAP2;
using TEXEngine::Asset::TEXENG_TEXTURE_BLENDMAP;

using TEXEngine::Util::TEXENG_RENDERING_WIREFRAME;
using TEXEngine::Util::TEXENG_RENDERING_FILL;

using TEXEngine::Util::TEXENG_MATRIX_MODELVIEW;
using TEXEngine::Util::TEXENG_MATRIX_PROJECTION;

namespace TEXEngine {
	namespace Port {

PCRenderer::PCRenderer()
	: m_pixel_format(0), m_hdc(0),
	  m_hrc(0), m_hwnd(0)
{}

PCRenderer::~PCRenderer()
{
	deinit();
}

int PCRenderer::init(HWND handle)
{
	// Fill in the pixel format descriptor
	ZeroMemory(&m_pfd, sizeof(m_pfd));

	m_pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);	// Size Of This Pixel Format Descriptor
	m_pfd.nVersion   = 1; 								// Version Number
	m_pfd.dwFlags    = PFD_DRAW_TO_WINDOW |				// Format Must Support Window
					   PFD_SUPPORT_OPENGL |				// Format Must Support OpenGL
					   PFD_DOUBLEBUFFER;				// Must Support Double Buffering
	m_pfd.iPixelType = PFD_TYPE_RGBA;					// Request An RGBA Format
	m_pfd.cColorBits = 24;								// Select Our Color Depth
	m_pfd.cDepthBits = 16;								// 16Bit Z-Buffer (Depth Buffer) 
	m_pfd.iLayerType = PFD_MAIN_PLANE;					// Main Drawing Layer

	m_hwnd = handle;

	// Did We Get A Device Context?
	if (!(m_hdc = GetDC(m_hwnd)))
		return 1;

	// Did Windows Find A Matching Pixel Format?
	if (!(m_pixel_format = ChoosePixelFormat(m_hdc, &m_pfd)))
		return 1;

	// Are We Able To Set The Pixel Format?
	if(!SetPixelFormat(m_hdc, m_pixel_format, &m_pfd))		
		return 1;

	// Are We Able To Get A Rendering Context?
	if (!(m_hrc=wglCreateContext(m_hdc)))				
		return 1;

	// Try To Activate The Rendering Context
	if(!wglMakeCurrent(m_hdc, m_hrc))
		return 1;

	// Turn vsync on.
	BOOL (APIENTRY *wglSwapIntervalEXT)(int);
	wglSwapIntervalEXT=(BOOL(APIENTRY*)(int))wglGetProcAddress("wglSwapIntervalEXT");
	wglSwapIntervalEXT(true);

	// Show the window
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	glewInit();
	return 0;
}

int PCRenderer::deinit()
{
	wglMakeCurrent(m_hdc, 0);
	wglDeleteContext(m_hrc);
	ReleaseDC(m_hwnd, m_hdc);

	// Release all opengl buffers
	for (std::vector<GFXAPIuint>::iterator it = m_texture_ids.begin(); it != m_texture_ids.end(); it++)
		glDeleteTextures(1, &(*it));

	for (std::vector<GFXAPIuint>::iterator it = m_buffer_ids.begin(); it != m_buffer_ids.end(); it++)
		glDeleteBuffers(1, &(*it));

	return 0;
}
		
void PCRenderer::scene_begin()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Correct the perspective projection in relation to the 
	// window ratio.
	GLfloat xmin, xmax, ymin, ymax;
	GLfloat zmin = 0.1f, zmax = 1000.0f;
	GLfloat aspect = (GLfloat)viewport_width() / (GLfloat)viewport_height();
	GLfloat fov = 45.0f * aspect;
	ymax = zmin * tanf(fov * (GLfloat)3.14159 / 360.0f);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;

	glFrustum(xmin, xmax, ymin, ymax, zmin, zmax);

	/*
	gluPerspective(45.0f, (GLfloat)viewport_width() / (GLfloat)viewport_height(), 
		0.1f, 1000.0f);
	*/

	glViewport(0, 0, viewport_width(), viewport_height());

	glClearColor(clear_color().red, clear_color().green, clear_color().blue, clear_color().alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void PCRenderer::scene_end()
{
	SwapBuffers(m_hdc);
}

void PCRenderer::set_rendering_mode(TEXENG_RENDERING_TYPE mode)
{
	switch (mode) {
		case TEXENG_RENDERING_WIREFRAME:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case TEXENG_RENDERING_FILL:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
	}
}

// Matrix operations
void PCRenderer::set_matrix_mode(TEXENG_MATRIX_TYPE mode)
{
	switch(mode)
	{
		case TEXENG_MATRIX_MODELVIEW:
			glMatrixMode(GL_MODELVIEW);
			break;

		case TEXENG_MATRIX_PROJECTION:
			glMatrixMode(GL_PROJECTION);
			break;
	}	
}

void PCRenderer::set_matrix(const Matrix4x4f *matrix)
{
	float temp_mat_array[16];
	matrix_to_1D_array(temp_mat_array, matrix);

	glLoadMatrixf(temp_mat_array);
}

void PCRenderer::multiply_matrix(const Matrix4x4f &matrix)
{
	//matrix
	float temp_mat_array[16];
	matrix_to_1D_array(temp_mat_array, matrix);
	glMultMatrixf(temp_mat_array);
}

void PCRenderer::push_matrix()
{
	glPushMatrix();
}

void PCRenderer::pop_matrix()
{
	glPopMatrix();
}

void PCRenderer::set_matrix_identity()
{
	glLoadIdentity();
}

void PCRenderer::draw_line(const Vector3f &from, const Vector3f &to, const ColorRGBAf &color)
{
	glBegin(GL_LINES);
		glColor4f((GLfloat)color.red, (GLfloat)color.green, (GLfloat)color.blue, (GLfloat)color.alpha);
		glVertex3f((GLfloat)from.x, (GLfloat)from.y, (GLfloat)from.z);
		glVertex3f((GLfloat)to.x, (GLfloat)to.y, (GLfloat)to.z);
	glEnd();
}

void PCRenderer::initiate_mesh(Mesh *mesh)
{
	if (!mesh)
		return;
	
	if (!mesh->vbo()) {
		GLuint vbo;
		glGenBuffersARB(1, &vbo);
		glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo);
		glBufferDataARB(GL_ARRAY_BUFFER_ARB, mesh->vertices().count() * sizeof(vertex_t), 
			mesh->vertices().data(), mesh->is_dynamic() ? GL_DYNAMIC_DRAW_ARB : GL_STATIC_DRAW);

		mesh->vbo(vbo);
		m_buffer_ids.push_back(vbo);

		TEXEngine::Core::LogManager::get().log_message("Initiated GL VBO, id: %i",mesh->vbo());
	}

	if (!mesh->ibo()) {
		GLuint ibo;
		glGenBuffersARB(1, &ibo);
		glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, ibo);
		glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mesh->indices().count() * sizeof(index_t), 
			mesh->indices().data(), GL_STATIC_DRAW_ARB);

		mesh->ibo(ibo);
		m_buffer_ids.push_back(ibo);

		TEXEngine::Core::LogManager::get().log_message("Initiated GL IBO, id: %i",mesh->ibo());
	}

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mesh->vbo());
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mesh->ibo());

	if(mesh->is_damaged() && mesh->is_dynamic()) {
		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, mesh->vertices().count() * sizeof(vertex_t), 
			mesh->vertices().data());
		mesh->undamage();
	}
}

void PCRenderer::deinitiate_mesh(Mesh *mesh)
{
	if (!mesh)
		return;

	GLuint vbo = mesh->vbo();
	GLuint ibo = mesh->ibo();
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);

	std::vector<GFXAPIuint>::iterator it;

	for(it = m_buffer_ids.begin(); it != m_buffer_ids.end(); it++)
		if ((*it) == vbo) {
			m_buffer_ids.erase(it);
			break;
		}

	for(it = m_buffer_ids.begin(); it != m_buffer_ids.end(); it++)
		if ((*it) == ibo) {
			m_buffer_ids.erase(it);
			break;
		}

	mesh->vbo(0);
	mesh->ibo(0);
}

void PCRenderer::render_mesh(Mesh *mesh)
{
	if (!mesh)
		return;

	initiate_mesh(mesh);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, mesh->vbo());
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mesh->ibo());

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	// Attribute arrays for tangents and binormals.
	glEnableVertexAttribArrayARB(14);
	
	glTexCoordPointer(2, GL_FLOAT, sizeof(vertex_t), (char*)NULL + (3*sizeof(GLfloat)));
	glNormalPointer(GL_FLOAT, sizeof(vertex_t), (char*)NULL + (5*sizeof(GLfloat)));
	glVertexAttribPointer(14, 3, GL_FLOAT, GL_TRUE, sizeof(vertex_t), (char*)NULL + (8*sizeof(GLfloat)));
	glColorPointer(4, GL_FLOAT, sizeof(vertex_t), (char*)NULL + (11*sizeof(GLfloat)));
	glVertexPointer(3, GL_FLOAT, sizeof(vertex_t), (char*)NULL);

	glDrawElements(GL_TRIANGLES, mesh->indices().count(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArrayARB(14);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

	/*
		In shader:
		struct MyVert{
		    float3 pos : POSITION;
			float3 normal : NORMAL;
			float3 tangent : ATTR14;
		}
	*/
}

void PCRenderer::initiate_texture(Texture *texture)
{
	// Check for invalid pointer
	if (!texture)
		return;

	// Check for invalid data
	if(!texture->img.pixels().count())
		return;

	// Check if the texture was generated
	if(texture->id())
		return;

	glEnable(GL_TEXTURE_2D);

	switch (texture->type()) {
		case TEXENG_TEXTURE_GENERIC:
		case TEXENG_TEXTURE_SPRITE:
		case TEXENG_TEXTURE_FONT:
		case TEXENG_TEXTURE_COLORMAP:
			glActiveTextureARB(GL_TEXTURE0);
			break;
		case TEXENG_TEXTURE_HEIGHTMAP:
			glActiveTextureARB(GL_TEXTURE1);
			break;
		case TEXENG_TEXTURE_NORMALMAP1:
			glActiveTextureARB(GL_TEXTURE2);
			break;
		case TEXENG_TEXTURE_NORMALMAP2:
			glActiveTextureARB(GL_TEXTURE3);
			break;
		case TEXENG_TEXTURE_BLENDMAP:
			glActiveTextureARB(GL_TEXTURE4);
			break;
	}

	GLuint t_id;
	glGenTextures(1, &t_id);
	glBindTexture(GL_TEXTURE_2D, t_id);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	unsigned int cnt = texture->img.pixels().count();
	ColorRGBAf *buf = texture->img.pixels().data();
		
	GLfloat *pdata = new (std::nothrow) GLfloat[cnt * 4];

	if(!pdata)
		return;

	for (unsigned int i = 0; i < cnt; i++) {
		pdata[i * 4 + 0] = buf[i].red;
		pdata[i * 4 + 1] = buf[i].green;
		pdata[i * 4 + 2] = buf[i].blue;
		pdata[i * 4 + 3] = buf[i].alpha;
	}

	if(!gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, 
		texture->img.width(), texture->img.height(), 
		GL_RGBA, GL_FLOAT, pdata)) {	
		texture->id(t_id);
		m_texture_ids.push_back(texture->id());
		TEXEngine::Core::LogManager::get().log_message("Initiated GL Texture, id: %i", texture->id());
	} else
		TEXEngine::Core::LogManager::get().log_message("Failed to initiate GL Texture.");
		
	delete [] pdata;

	glDisable(GL_TEXTURE_2D);
}

void PCRenderer::deinitiate_texture(Texture *texture)
{
	GLuint tex = texture->id();
	glDeleteTextures( 1, &tex);

	std::vector<GFXAPIuint>::iterator it;
	for(it = m_texture_ids.begin(); it != m_texture_ids.end(); it++)
		if ((*it) == tex) {
			m_texture_ids.erase(it);
			break;
		}

	texture->id(0);
}

void PCRenderer::activate_texture(Texture *texture)
{
	if (!texture)
		return;

	glEnable(GL_TEXTURE_2D);

	switch (texture->type()) {
		case TEXENG_TEXTURE_GENERIC:
		case TEXENG_TEXTURE_SPRITE:
		case TEXENG_TEXTURE_FONT:
		case TEXENG_TEXTURE_COLORMAP:
			glActiveTextureARB(GL_TEXTURE0);
			break;
		case TEXENG_TEXTURE_HEIGHTMAP:
			glActiveTextureARB(GL_TEXTURE1);
			break;
		case TEXENG_TEXTURE_NORMALMAP1:
			glActiveTextureARB(GL_TEXTURE2);
			break;
		case TEXENG_TEXTURE_NORMALMAP2:
			glActiveTextureARB(GL_TEXTURE3);
			break;
		case TEXENG_TEXTURE_BLENDMAP:
			glActiveTextureARB(GL_TEXTURE4);
			break;
	}

	glBindTexture(GL_TEXTURE_2D, texture->id());

	// Check if the texture was generated
	if(!texture->id()) {
		initiate_texture(texture);				
	}
}

void PCRenderer::deactivate_texture()
{
	glActiveTextureARB(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, 0);	
	glActiveTextureARB(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, 0);	
	glActiveTextureARB(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTextureARB(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTextureARB(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void PCRenderer::render_box(float x, float y, float w, float h, 
	const ColorRGBAf &col, int texid,
	const Vector2f &uv1, const Vector2f &uv2, const Vector2f &uv3, const Vector2f &uv4)
{
	glDisable(GL_DEPTH_TEST);
	float aspect = (float)viewport_width() / (float)viewport_height();
    glViewport(0, 0, viewport_width(), viewport_height());

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-aspect / aspect_modifier_2d(), aspect / aspect_modifier_2d(), -1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	Texture *tex = AssetManager::get().get_texture(texid);

	if(tex) {
		if(!tex->id())
			initiate_texture(tex);

		glEnable(GL_TEXTURE_2D);
		glActiveTextureARB(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex->id());
	}

	//glEnable(GL_BLEND);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();

	glBegin(GL_QUADS);
		if(!tex)
			glColor4f(col.red, col.green, col.blue, col.alpha);
		else
			glColor4f(1.0f, 1.0f, 1.0f, col.alpha);

		glTexCoord2f((GLfloat)uv1.x, (GLfloat)uv1.y);
		glVertex2f(x, y);

		glTexCoord2f((GLfloat)uv2.x, (GLfloat)uv2.y);
		glVertex2f(x + w, y);

		glTexCoord2f((GLfloat)uv3.x, (GLfloat)uv3.y);
		glVertex2f(x + w, y + h);

		glTexCoord2f((GLfloat)uv4.x, (GLfloat)uv4.y);
		glVertex2f(x, y + h);
	glEnd();
	
	glPopMatrix();

	//glDisable(GL_BLEND);
	
	if(tex) {
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}

	glEnable(GL_DEPTH_TEST);
}

	} /* namespace Port */
} /* namespace TEXEngine */

#endif /* _WIN32 */