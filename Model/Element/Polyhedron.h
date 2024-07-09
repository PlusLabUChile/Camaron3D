#ifndef POLYHEDRON_H
#define POLYHEDRON_H
#include "Common/Constants.h"
#include <vector>
#include <glm/glm.hpp>
#include "Model/Element/Element.h"
namespace vis{
class Polygon;
class Vertex;
class Polyhedron: public Element
{
	public:
		Polyhedron( int );
		virtual ~Polyhedron();
		glm::vec3 getGeometricCenter();
		//int getPolygonsCount();
		void setGeometricCenter(glm::vec3);
		///@unused 
		float getArea();
		void setArea(float);
		///
		float getVolume();
		void setVolumen(float);
		//bool isAtSurface();
		glm::vec3 getInwardNormal(vis::Polygon*, vis::Vertex*);
		glm::vec3 getOutwardNormal(vis::Polygon*, vis::Vertex*);

		DOUBLE_DISPATCH_ELEMENT_DEC
	protected:
		glm::vec3 geoCenter;
		float area;
		float volumen;
};
}
#endif // POLYHEDRON_H
