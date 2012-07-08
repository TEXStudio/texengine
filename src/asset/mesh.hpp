#ifndef TEXENGINE_MESH_HPP_INCLUDED
#define TEXENGINE_MESH_HPP_INCLUDED

#include "../port/gfxapi.h"
#include "../port/vertex.h"
#include "../util/buffer.hpp"

using TEXEngine::Port::GFXAPIuint;
using TEXEngine::Port::vertex_t;
using TEXEngine::Port::index_t;
using TEXEngine::Util::Buffer;

namespace TEXEngine {

	namespace Core {
		class AssetManager; /* Forward declaration */
	}

	namespace Port {
		class PCRenderer; /* Forward declaration */
	}

	using Core::AssetManager;
	using Port::PCRenderer;

	namespace Asset {

class Mesh
{
	friend class AssetManager;
	friend class PCRenderer;

	public:
		// RETURN CODES:
		//	0. Everything went well.
		//  1. Failed to initialize vertices.
		//  2. Failed to initialize indices.
		//  3. Failed to initialize both.
		int init(unsigned int vcount, unsigned int icount);
		
		const Buffer<vertex_t> &vertices() const;
		const Buffer<index_t> &indices() const;

		Buffer<vertex_t> &vertices_w();
		Buffer<index_t> &indices_w();

		void damage();
		void dynamic(bool status);
		void visible(bool status);

		bool is_damaged() const;
		bool is_dynamic() const;
		bool is_visible() const;
		
	protected:
		Mesh();
		Mesh(const Mesh &m);
		Mesh &operator =(const Mesh &m);
		~Mesh();

		void undamage();

		void vbo(GFXAPIuint id);
		void ibo(GFXAPIuint id);
		GFXAPIuint vbo();
		GFXAPIuint ibo();

	private:
		Buffer<vertex_t> m_vertices;
		Buffer<index_t> m_indices;
		
		bool m_is_dynamic;
		bool m_is_damaged;
		bool m_is_visible;

		GFXAPIuint m_vbo;
		GFXAPIuint m_ibo;
};

	} /* namespace Asset */
} /* Namespace TEXEngine */

#endif /* TEXENGINE_MESH_HPP_INCLUDED */