#include "buffer.hpp"
#include "image.hpp"
#include "../math/vector.hpp"
#include "../port/vertex.h"
#include "ppm.hpp"
#include "heightmap.hpp"


#include "../log/logmanager.hpp"
using TEXEngine::Core::LogManager;

using TEXEngine::Util::ColorRGBAf;
using TEXEngine::Util::Image;
using TEXEngine::Util::IO::Import::ppm_raw;

namespace TEXEngine {
	namespace Util {
		namespace Mutator {

int heightmap(Mesh *mesh, const char *heightmap, const float base, const float multiplier)
{
	if(!mesh)
		return 1;

	if(!heightmap)
		return 2;

	Image img;

	// Load the heightmap
	if(ppm_raw(heightmap, &img))
		return 3;

	const unsigned int w = img.width();
	const unsigned int h = img.height();

	Buffer<vertex_t> *p_meshv = &(mesh->vertices_w());

	for (unsigned int i = 0; i < mesh->vertices().count(); i++) {
		// Get vertex uv
		float vu = p_meshv->data()[i].u > 1.0f ? 1.0f : p_meshv->data()[i].u ;
		float vv = p_meshv->data()[i].v > 1.0f ? 1.0f : p_meshv->data()[i].v ;

		// Get pixel coords
		float u = floor(vu * (float)w);
		float v = floor(vv * (float)h);

		// Get pixel
		ColorRGBAf col = img.pixel((unsigned int)u, (unsigned int)v);
		float res = float(col.red + col.green + col.blue);
		
		// Modulate the mesh
		p_meshv->data()[i].x += p_meshv->data()[i].nx * (res - base) * multiplier;
		p_meshv->data()[i].y += p_meshv->data()[i].ny * (res - base) * multiplier;
		p_meshv->data()[i].z += p_meshv->data()[i].nz * (res - base) * multiplier;
	}	

	return 0;
}

		} /* namespace Mutator */
	} /* namespace Util */
} /* namespace TEXEngine */