#ifndef TEXENGINE_OBJ_HPP_INCLUDED
#define TEXENGINE_OBJ_HPP_INCLUDED

#include "../asset/mesh.hpp"

using namespace TEXEngine::Asset;

namespace TEXEngine {
	namespace Util {
		namespace IO {
			namespace Import {

// RESTRICTIONS:
// When exporting the obj file you MUST
// select to triangulate the mesh and export
// uv coordinates and indices. Incompatible
// formats will be treated as having erroneous
// syntax.
//
// RETURN CODES:
// 0. Everything went well.
// 1. filename equals NULL.
// 2. mesh equals NULL.
// 3. Failed to open file.
// 4. Invalid format.
// 5. Failed to initialize the Mesh.
int obj(const char *filename, Mesh *mesh);

			} /* namespace Import */

			namespace Export {
			} /* namespace Export */

		} /* namespace IO */
	} /* namespace Util */
} /* namespace TEXEngine */

#endif /* TEXENGINE_OBJ_HPP_INCLUDED */
