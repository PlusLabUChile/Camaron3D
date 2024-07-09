#ifndef POLYGON_H
#define POLYGON_H

#include <glm/glm.hpp>
#include "Model/Element/Element.h"
#include <vector>

namespace vis{
class Vertex;
class Polyhedron;
class Polygon: public vis::Element
{
	public:
		Polygon( int );
		virtual ~Polygon();
		glm::vec3 getGeometricCenter();
		void setGeometricCenter(glm::vec3&);
		// virtual bool isNeighbor(Polygon* p);
		glm::vec3& getNormal();
		void setNormal(glm::vec3& value);
		void setArea(float value);
		float getArea();
		//bool isAtSurface();
		//virtual void invertVerticesOrder();

		DOUBLE_DISPATCH_ELEMENT_DEC
	protected:
		glm::vec3 normal;
		float area;
		glm::vec3 geoCenter;

	private:
};
}
#endif // POLYGON_H
