#ifndef TEXENGINE_GFXAPI_H_INCLUDED
#define TEXENGINE_GFXAPI_H_INCLUDED

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <Windows.h>
	#include <GL/GL.h>
#endif /* _WIN32 */

namespace TEXEngine {
	namespace Port {

#if defined(_WIN32)
	typedef GLenum			GFXAPIenum;
	typedef GLboolean		GFXAPIboolean;
	typedef GLbitfield		GFXAPIbitfield;
	typedef GLbyte			GFXAPIbyte;
	typedef GLshort			GFXAPIshort;
	typedef GLint			GFXAPIint;
	typedef GLsizei			GFXAPIsizei;
	typedef GLubyte			GFXAPIubyte;
	typedef GLushort		GFXAPIushort;
	typedef GLuint			GFXAPIuint;
	typedef GLfloat			GFXAPIfloat;
	typedef GLclampf		GFXAPIclampf;
	typedef GLdouble		GFXAPIdouble;
	typedef GLclampd		GFXAPIclampd;
	typedef GLvoid			GFXAPIvoid;
#endif /* _WIN32 */ 

	} /* namespace Port */
} /* namespace TEXEngine */

#endif /* TEXENGINE_GFXAPI_H_INCLUDED */