//#include <fstream>
#include <string>
#include <vector>
#include "buffer.hpp"
#include "../math/vector.hpp"
#include "../port/vertex.h"
#include "obj.hpp"

using namespace TEXEngine::Math;
using namespace TEXEngine::Port;

namespace TEXEngine {
	namespace Util {
		namespace IO {
			namespace Import {
/*
	NOTES:
	In this implementation I am using c-style I/O.
	This is due to our latest discovery that the PSP
	does not implement a large portion of STL including
	streams. Still undecided if it is worth implementing
	the missing STL utils, porting the rest of the code
	or to drop the psp version all together.

	This is implemented as a single pass parser.
	Due to the way that obj stores the vertex data
	some vertices will be duplicated. This can be
	avoided with post processing of the data, however
	due to time restrictions such an optimization
	is left for later on.
*/

/*
	FORMAT: OBJ (Restrictions apply. See header file for details.)
	TYPE: ASCII

	HEADER:
	No header data.
*/

int obj(const char* filename, Mesh *mesh)
{
	if(!filename)
		return 1;

	if(!mesh)
		return 2;

	FILE *fp = fopen(filename, "r");

	if (!fp)
		return 3;

	// Temporary data buffers
	std::vector<Vector2f> uv_coord;
	std::vector<Vector3f> position;
	std::vector<Vector3f> normal;

	std::vector<vertex_t> vertices;
	std::vector<index_t> indices;

	unsigned int idx = 0;

	// Import the data
	char line[512];
	// Zero out the buffer.
	memset(line, '\0', sizeof(line) * sizeof(char));

	while(fgets(line,sizeof(line),fp)) {
		if (!line[0] || line[0] == '#')
			continue;

		bool syntax_error = false; // Flag to check for syntax errors

		vertex_t vert;
		Vector2f uv;
		float x, y, z, u, v;
		unsigned int i1, i2, i3, i4, i5, i6, i7, i8, i9;

		switch (line[0]) {
			case 'v':
			{
				if (line[1]) {
					switch (line[1]) { 
						case ' ':
						{
							if(sscanf(line, "v %f %f %f", 
								&x, &y, &z) != 3) {
								syntax_error = true;
								break;
							}
							position.push_back(Vector3f(x, y, z));
							break;
						}
						case 't':
						{
							if(sscanf(line, "vt %f %f", 
								&(u), &(v)) != 2) {
								syntax_error = true;
								break;
							}
							// Make the uv coordinates
							uv_coord.push_back(Vector2f(u, 1.0f - v));
							break;
						}
						case 'n':
						{
							if(sscanf(line, "vn %f %f %f", 
								&x, &y, &z) != 3) {
								syntax_error = true;
								break;
							}
							// Make the uv coordinates
							normal.push_back(Vector3f(x, y, z));
							break;
						}
					}
				}
				break;
			}
			case 'f':
			{
				if(sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
						&i1, &i2, &i3, &i4, &i5, &i6, &i7, &i8, &i9) != 9)	{
					syntax_error = true;
					break;
				}

				// This is an important security check for malformed files.
				if(	   i1 > position.size()
					|| i2 > uv_coord.size()
					|| i3 > normal.size()
					|| i4 > position.size()
					|| i5 > uv_coord.size()
					|| i6 > normal.size()
					|| i7 > position.size()
					|| i8 > uv_coord.size()
					|| i9 > normal.size()
					) {
					syntax_error = true;
					break;
				}

				// 1st vertex
				vert.x = (GFXAPIfloat)position[i1-1].x;
				vert.y = (GFXAPIfloat)position[i1-1].y;
				vert.z = (GFXAPIfloat)position[i1-1].z;

				vert.r = 1.0f;
				vert.g = 1.0f;
				vert.b = 1.0f;
				vert.a = 1.0f;

				vert.u = (GFXAPIfloat)uv_coord[i2-1].x;
				vert.v = (GFXAPIfloat)uv_coord[i2-1].y;
				
				vert.nx = (GFXAPIfloat)normal[i3-1].x;
				vert.ny = (GFXAPIfloat)normal[i3-1].y;
				vert.nz = (GFXAPIfloat)normal[i3-1].z;

				vertices.push_back(vert);
				indices.push_back(idx);

				// 2nd vertex
				vert.x = (GFXAPIfloat)position[i4-1].x;
				vert.y = (GFXAPIfloat)position[i4-1].y;
				vert.z = (GFXAPIfloat)position[i4-1].z;

				vert.r = 1.0f;
				vert.g = 1.0f;
				vert.b = 1.0f;
				vert.a = 1.0f;

				vert.u = (GFXAPIfloat)uv_coord[i5-1].x;
				vert.v = (GFXAPIfloat)uv_coord[i5-1].y;

				vert.nx = (GFXAPIfloat)normal[i6-1].x;
				vert.ny = (GFXAPIfloat)normal[i6-1].y;
				vert.nz = (GFXAPIfloat)normal[i6-1].z;
	
				vertices.push_back(vert);
				indices.push_back(idx+1);

				// 3rd vertex
				vert.x = (GFXAPIfloat)position[i7-1].x;
				vert.y = (GFXAPIfloat)position[i7-1].y;
				vert.z = (GFXAPIfloat)position[i7-1].z;

				vert.r = 1.0f;
				vert.g = 1.0f;
				vert.b = 1.0f;
				vert.a = 1.0f;

				vert.u = (GFXAPIfloat)uv_coord[i8-1].x;
				vert.v = (GFXAPIfloat)uv_coord[i8-1].y;

				vert.nx = (GFXAPIfloat)normal[i9-1].x;
				vert.ny = (GFXAPIfloat)normal[i9-1].y;
				vert.nz = (GFXAPIfloat)normal[i9-1].z;

				vertices.push_back(vert);
				indices.push_back(idx+2);

				idx += 3;
				break;
			}
		}

		if (syntax_error) {
			fclose(fp);
			return 4;
		}

		// Zero out the buffer.
		memset(line, '\0', sizeof(line) * sizeof(char));
	}

	fclose(fp);

	// Update the data in the mesh buffers.
	if(mesh->init(vertices.size(), indices.size()))
		return 5;

	Buffer<vertex_t> *p_meshv = &(mesh->vertices_w());
	Buffer<index_t> *p_meshi = &(mesh->indices_w());

	for (unsigned int i = 0; i < vertices.size(); i++)
		p_meshv->data()[i] = vertices[i];

	for (unsigned int i = 0; i < indices.size(); i++)
		p_meshi->data()[i] = indices[i];

	return 0;
}

			} /* namespace Import */

			namespace Export {
			} /* namespace Export */

		} /* namespace IO */
	} /* namespace Util */
} /* namespace TEXEngine */