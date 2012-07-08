#include "buffer.hpp"
#include "../math/vector.hpp"
#include "../port/vertex.h"
#include "calcnorm.hpp"

using TEXEngine::Math::scalar_t;
using TEXEngine::Math::Vector2f;
using TEXEngine::Math::Vector3f;
using TEXEngine::Math::dot;
using TEXEngine::Math::cross;
using TEXEngine::Port::vertex_t;
using TEXEngine::Port::index_t;
using TEXEngine::Port::GFXAPIfloat;

namespace TEXEngine {
	namespace Util {
		namespace Mutator {

int calc_tangent(Mesh *mesh)
{
	if(!mesh)
		return 1;

	Buffer<vertex_t> *p_meshv = &(mesh->vertices_w());
	Buffer<index_t> *p_meshi = &(mesh->indices_w());

	// Zero out all the tangents and binormals.
	for (unsigned int i = 0; i < p_meshv->count(); i++) {
		p_meshv->data()[i].tx = 0.0f;
		p_meshv->data()[i].ty = 0.0f;
		p_meshv->data()[i].tz = 0.0f;
	/*
		p_meshv->data()[i].bx = 0.0f;
		p_meshv->data()[i].by = 0.0f;
		p_meshv->data()[i].bz = 0.0f;
	*/
	}


	for (unsigned int i = 0; i < p_meshi->count(); i+=3) {
		// Triangle vertex positions.
		Vector3f v0 = Vector3f(
			p_meshv->data()[p_meshi->data()[i  ]].x, 
			p_meshv->data()[p_meshi->data()[i  ]].y, 
			p_meshv->data()[p_meshi->data()[i  ]].z);
		
		Vector3f v1 = Vector3f(
			p_meshv->data()[p_meshi->data()[i+1]].x, 
			p_meshv->data()[p_meshi->data()[i+1]].y, 
			p_meshv->data()[p_meshi->data()[i+1]].z);

		Vector3f v2 = Vector3f(
			p_meshv->data()[p_meshi->data()[i+2]].x, 
			p_meshv->data()[p_meshi->data()[i+2]].y, 
			p_meshv->data()[p_meshi->data()[i+2]].z);

		// This might need to be calculated in opposite order.
		
		// Texture UVs.
		Vector2f w0 = Vector2f(
			p_meshv->data()[p_meshi->data()[i  ]].u, 
			p_meshv->data()[p_meshi->data()[i  ]].v);
		
		Vector2f w1 = Vector2f(
			p_meshv->data()[p_meshi->data()[i+1]].u, 
			p_meshv->data()[p_meshi->data()[i+1]].v);
		
		Vector2f w2 = Vector2f(
			p_meshv->data()[p_meshi->data()[i+2]].u, 
			p_meshv->data()[p_meshi->data()[i+2]].v);


		// triangle normal
		Vector3f a = v2 - v0;
		Vector3f b = v1 - v0;

		// Calculate the cross product
		Vector3f fnormal = cross(b, a);
		fnormal.normalize();

		scalar_t x0 = v1.x - v0.x;
		scalar_t x1 = v2.x - v0.x;
		scalar_t y0 = v1.y - v0.y;
		scalar_t y1 = v2.y - v0.y;
		scalar_t z0 = v1.z - v0.z;
		scalar_t z1 = v2.z - v0.z;
  
		scalar_t s0 = w1.x - w0.x;
		scalar_t s1 = w2.x - w0.x;
		scalar_t t0 = w1.y - w0.y;
		scalar_t t1 = w2.y - w0.y;

		scalar_t r = 1.0f / (s0 * t1 - s1 * t0);

		Vector3f sdir((t1 * x0 - t0 * x1) * r, (t1 * y0 - t0 * y1) * r, (t1 * z0 - t0 * z1) * r);
		Vector3f tdir((s0 * x1 - s1 * x0) * r, (s0 * y1 - s1 * y0) * r, (s0 * z1 - s1 * z0) * r);

		// Tangent. Gram-Schmidt orthogonalization.
		Vector3f tangent = sdir - fnormal * dot(fnormal, sdir);   
		tangent.normalize();   
	/*	
		scalar_t tangentdir = (dot(cross(fnormal, sdir), tdir) >= 0.0f) ? 1.0f : -1.0f;
		Vector3f binormal = cross(fnormal, tangent) * tangentdir;
	*/
		// Modify the per-vertex tangents.
		p_meshv->data()[p_meshi->data()[i]].tx += (GFXAPIfloat)tangent.x;
		p_meshv->data()[p_meshi->data()[i]].ty += (GFXAPIfloat)tangent.y;
		p_meshv->data()[p_meshi->data()[i]].tz += (GFXAPIfloat)tangent.z;

		p_meshv->data()[p_meshi->data()[i+1]].tx += (GFXAPIfloat)tangent.x;
		p_meshv->data()[p_meshi->data()[i+1]].ty += (GFXAPIfloat)tangent.y;
		p_meshv->data()[p_meshi->data()[i+1]].tz += (GFXAPIfloat)tangent.z;

		p_meshv->data()[p_meshi->data()[i+2]].tx += (GFXAPIfloat)tangent.x;
		p_meshv->data()[p_meshi->data()[i+2]].ty += (GFXAPIfloat)tangent.y;
		p_meshv->data()[p_meshi->data()[i+2]].tz += (GFXAPIfloat)tangent.z;
	/*
		// Modify the per-vertex binormals.
		p_meshv->data()[p_meshi->data()[i]].bx += (GFXAPIfloat)binormal.x;
		p_meshv->data()[p_meshi->data()[i]].by += (GFXAPIfloat)binormal.y;
		p_meshv->data()[p_meshi->data()[i]].bz += (GFXAPIfloat)binormal.z;

		p_meshv->data()[p_meshi->data()[i+1]].bx += (GFXAPIfloat)binormal.x;
		p_meshv->data()[p_meshi->data()[i+1]].by += (GFXAPIfloat)binormal.y;
		p_meshv->data()[p_meshi->data()[i+1]].bz += (GFXAPIfloat)binormal.z;

		p_meshv->data()[p_meshi->data()[i+2]].bx += (GFXAPIfloat)binormal.x;
		p_meshv->data()[p_meshi->data()[i+2]].by += (GFXAPIfloat)binormal.y;
		p_meshv->data()[p_meshi->data()[i+2]].bz += (GFXAPIfloat)binormal.z;
	*/
	}

	for (unsigned int i = 0; i < p_meshv->count(); i++) {
		Vector3f vt(p_meshv->data()[i].tx, 
				    p_meshv->data()[i].ty, 
				    p_meshv->data()[i].tz);
	/*
		Vector3f vb(p_meshv->data()[i].bx, 
				    p_meshv->data()[i].by, 
				    p_meshv->data()[i].bz);
	*/
		vt.normalize();

	/*	
		vb.normalize();
	*/
		
		p_meshv->data()[i].tx = (GFXAPIfloat)vt.x;
		p_meshv->data()[i].ty = (GFXAPIfloat)vt.y;
		p_meshv->data()[i].tz = (GFXAPIfloat)vt.z;
	/*
		p_meshv->data()[i].bx = (GFXAPIfloat)vb.x;
		p_meshv->data()[i].by = (GFXAPIfloat)vb.y;
		p_meshv->data()[i].bz = (GFXAPIfloat)vb.z;
	*/
	}

	// In the shader compute the 3x3 tranform from world space to tangent space as:
	// scalar_t3x3 objToTangentSpace;   
	// objToTangentSpace[0] = Tangent;   
	// objToTangentSpace[1] = Binormal;   
	// objToTangentSpace[2] = Normal;   

	return 0;
}

		} /* namespace Mutator */
	} /* namespace Util */
} /* namespace TEXEngine */