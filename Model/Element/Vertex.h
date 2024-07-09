#ifndef VERTEX_H
#define VERTEX_H
#include "Model/Element/Element.h"
#include <vector>
#include <glm/glm.hpp>
#include <limits>

namespace vis{
class Polygon;
class Vertex: public Element {
	public:
		Vertex( unsigned int id = 0, float x = 0.0f, float y = 0.0f, float z = 0.0f, unsigned int pos = NULL_POS);
		virtual ~Vertex();
		glm::vec3& getCoords();
		glm::vec3 getGeometricCenter();
		glm::vec3& getNormal();
		void setNormal(glm::vec3);
		//bool isAtSurface();
		static const unsigned int NULL_POS = std::numeric_limits<unsigned int>::max() ;
		DOUBLE_DISPATCH_ELEMENT_DEC
	protected:
	private:
		glm::vec3 coords;
		glm::vec3 normal;
};
}
#endif // VERTEX_H
