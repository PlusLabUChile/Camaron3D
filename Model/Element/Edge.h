#ifndef EDGE_H
#define EDGE_H
#include "Model/Element/Element.h"

namespace vis{
class Vertex;
class Edge : public vis::Element
{
	public:
		Edge(int idn);
		Edge(int idn, glm::vec3 color);
		virtual ~Edge();
		void setColor(glm::vec3 color);
		glm::vec3& getColor();
		bool isAtSurface();
		glm::vec3 getGeometricCenter();
		void setGeometricCenter(glm::vec3);
		DOUBLE_DISPATCH_ELEMENT_DEC
	private:
		glm::vec3 color;
		glm::vec3 geoCenter;

};
}

#endif // EDGE_H
