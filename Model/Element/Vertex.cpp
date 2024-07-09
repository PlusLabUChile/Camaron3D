#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "SelectionStrategies/SelectionStrategy.h"
#include "EvaluationStrategies/EvaluationStrategy.h"

vis::Vertex::Vertex( unsigned int idn, float xn, float yn, float zn, unsigned int npos ):
	Element( idn ),
	coords(xn,yn,zn),
	normal(0.0f,0.0f,0.0f){}

vis::Vertex::~Vertex() {
}
glm::vec3& vis::Vertex::getCoords(){ return coords; }

glm::vec3& vis::Vertex::getNormal(){
	/*if(recalculate || !normalCalculated){
		normal = glm::vec3(0.0f,0.0f,0.0f);
		typedef std::vector<vis::Polygon*>::size_type local_size_type;
		local_size_type nPolygons = this->polygons.size();
		int np = 0;
		if(nPolygons > 0){
			normalCalculated = true;
			for( local_size_type i = 0; i < nPolygons; i++ ){
				if(polygons[i]->isAtSurface()){
					this->normal += this->polygons[i]->getNormal();
					np++;
				}
			}
			if(np>0){
				this->normal = glm::normalize(normal);
			}
		}
	}*/
	return normal;
}
void vis::Vertex::setNormal(glm::vec3 newNormal){
	normal = newNormal;
}

DOUBLE_DISPATCH_ELEMENT_DEF(vis::Vertex)

glm::vec3 vis::Vertex::getGeometricCenter(){
	return coords;
}

