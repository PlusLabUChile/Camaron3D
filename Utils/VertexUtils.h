#ifndef VERTEXUTILS_H
#define VERTEXUTILS_H

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

/**
 * @brief Implementation of different functions called in differents components that gives essential information, for example,
 * to draw the model or to know if a vertex is at surface.
*/
class VertexUtils
{
	public:
		static bool isVertexAtSurface(Model*, vis::Vertex*);
		static void calculateGeometricCenter(Model* , vis::Edge*);
	protected:
	private:
		VertexUtils();
		virtual ~VertexUtils();
};

#endif // VERTEXUTILS_H
