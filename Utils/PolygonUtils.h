#ifndef POLYGONUTILS_H
#define POLYGONUTILS_H

#include <vector>
#include <glm/glm.hpp>
#include "GL/glew.h"
#include "Model/Model.h"

namespace vis{
class Element;
class Polygon;
class Vertex;
}
class Model;
class RVertexFlagAttribute;
class PolygonUtils
{
	public:
		static float calculateArea(Model*, vis::Polygon*);
		static glm::vec3 calculateNormal(Model*, vis::Polygon*);
		static float getDihedralAngle( vis::Polygon*,vis::Polygon* );
		static void getTriangleIndices(Model* model, vis::Polygon*, std::vector<GLuint>&);
		static void getTriangleVertices(Model* model, vis::Polygon*,
										std::vector<RVertexFlagAttribute>& flagAttributes);
		static void orderVerticesToCCWPolygon(std::vector<glm::vec3>&);

		static bool isPolygonNeighbor(Model*, vis::Polygon*);
		static bool isPolygonAtSurface(Model*, vis::Polygon*);
		static void invertVerticesOrder(Model*, vis::Polygon*);
		static glm::vec3 calculateGeometricCenter(Model*, vis::Polygon*);

	protected:
	private:
		static void configRVertexFlagAttribute(Model* model, vis::Vertex* element, RVertexFlagAttribute& attribute);
		PolygonUtils();
		virtual ~PolygonUtils();
};

#endif // POLYGONUTILS_H
