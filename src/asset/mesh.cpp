#include <cstring>
#include "mesh.hpp"

namespace TEXEngine {
	namespace Asset {

Mesh::Mesh()
	: m_is_dynamic(false),
	  m_is_visible(true),
	  m_is_damaged(true),
	  m_vbo(0),
	  m_ibo(0)
{}

Mesh::Mesh(const Mesh &m)
{
	if (&m == this)
		return;

	m_is_dynamic = m.is_dynamic();
	m_is_visible = m.is_visible();
	m_is_damaged = true;

	m_vertices = m.vertices();
	m_indices = m.indices();

	m_vbo = 0;
	m_ibo = 0;
}

Mesh &Mesh::operator =(const Mesh &m)
{
	if (&m == this)
		return *this;

	m_is_dynamic = m.is_dynamic();
	m_is_visible = m.is_visible();
	m_is_damaged = true;
	
	m_vertices = m.vertices();
	m_indices = m.indices();

	m_vbo = 0;
	m_ibo = 0;

	return *this;
}

Mesh::~Mesh()
{}

int Mesh::init(unsigned int vcount, unsigned int icount)
{
	int status = 0;

	if(m_vertices.init(vcount))	status = 1;
	if(m_indices.init(icount)) status = (status ? 3 : 2);
	
	return status;
}

const Buffer<vertex_t> &Mesh::vertices() const
{
	return m_vertices;
}

const Buffer<index_t> &Mesh::indices() const
{
	return m_indices;
}

Buffer<vertex_t> &Mesh::vertices_w()
{
	return m_vertices;
}

Buffer<index_t> &Mesh::indices_w()
{
	return m_indices;
}

void Mesh::damage()
{
	if(is_dynamic())
		m_is_damaged = true;
}

void Mesh::undamage()
{
	m_is_damaged = true;
}

void Mesh::dynamic(bool status)
{
	m_is_dynamic = status;
}

void Mesh::visible(bool status)
{
	m_is_visible = status;
}

bool Mesh::is_damaged() const
{
	return m_is_damaged;
}

bool Mesh::is_dynamic() const
{
	return m_is_dynamic;
}

bool Mesh::is_visible() const
{
	return m_is_visible;
}

GFXAPIuint Mesh::vbo()
{
	return m_vbo;
}

GFXAPIuint Mesh::ibo()
{
	return m_ibo;
}

void Mesh::vbo(GFXAPIuint id)
{
	m_vbo = id;
}

void Mesh::ibo(GFXAPIuint id)
{
	m_ibo = id;
}

	} /* namespace TEXEngine */
} /* namespace Asset */