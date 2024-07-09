#include "Utils/VertexUtils.h"
#include "Model/Model.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Edge.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Utils/PolygonUtils.h"

VertexUtils::VertexUtils() {}
VertexUtils::~VertexUtils() {}

bool VertexUtils::isVertexAtSurface(Model* model, vis::Vertex* vertex){
	for( unsigned int polID : model->getElementsRelations()->getPolygonsVertexById(vertex->getId()) ){
        vis::Polygon* polygon = &model->getPolygons()[polID];
        if(PolygonUtils::isPolygonAtSurface(model, polygon))
			return true;
    }
	return false;
}

void VertexUtils::calculateGeometricCenter(Model* model, vis::Edge* edge){
    relationPair verticesEdges = model->getElementsRelations()->getVertexAdditionalEdgesById(edge->getId());
    vis::Vertex* vertex0 = &model->getVertices()[verticesEdges.first_value];
    vis::Vertex* vertex1 = &model->getVertices()[verticesEdges.second_value];
    edge->setGeometricCenter((vertex0->getCoords()+vertex1->getCoords())/2.0f);
}