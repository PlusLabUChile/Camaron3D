#ifndef POLYHEDRONUTILS_H
#define POLYHEDRONUTILS_H

#include <vector>
#include <glm/glm.hpp>
#include "GL/glew.h"
#include "Model/Model.h"
namespace vis {
class Vertex;
class Polyhedron;
}
class PolyhedronUtils
{
	public:
		static void calculateArea(Model* model, vis::Polyhedron*);
		static void calculateVolumen(Model* model, vis::Polyhedron*);
		static void calculateGeometricCenter(Model*, vis::Polyhedron*);
		static void getPolyhedronVertices(Model* model, vis::Polyhedron*, std::vector<unsigned int>&);
		static void setPolyhedronRModelPositions(Model* model, vis::Polyhedron*);
		static float getPolyhedronSolidAngleFromVertex(Model* model, vis::Polyhedron* p, vis::Vertex* v);
		static void getTetrahedronIndices(Model* model, vis::Polyhedron*, std::vector<GLuint>&);
		static bool isPolyhedronAtSurface(Model* model, vis::Polyhedron* polyhedron);
	private:
		PolyhedronUtils();
		static glm::vec3 getCoordsSphericalCoordinates(glm::vec3);
};

#endif // POLYHEDRONUTILS_H
