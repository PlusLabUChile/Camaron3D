#include "modelgeneralstaticscollector.h"
#include "Model/Model.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/Polygon.h"
#include <unordered_map>
ModelGeneralStaticsCollector::ModelGeneralStaticsCollector(){}

void ModelGeneralStaticsCollector::fillModelStatics(Model* model){
	if (model->getPolyhedronsCount() != 0){
		fillPolyhedronsStatics(model);
		}

	if (model->getPolygonsCount() != 0){
		fillPolygonsStatics(model);
		}

	fillVertexStatics(model->getVertices());
	displayModelBoundaries(model->getBounds());
	}

void ModelGeneralStaticsCollector::displayModelBoundaries(std::vector<float>& bounds){
	ModelStaticsItem item("Model Bounds");

	item.addStat("Min X: ",bounds[0]);
	item.addStat("Min Y: ",bounds[1]);
	item.addStat("Min Z: ",bounds[2]);
	item.addStat("Max X: ",bounds[3]);
	item.addStat("Max Y: ",bounds[4]);
	item.addStat("Max Z: ",bounds[5]);
	modelStatics.push_back(item);
	}

///@note Passing the container in case of extending this method (Instead of passing the size directly)
void ModelGeneralStaticsCollector::fillVertexStatics(std::vector<vis::Vertex>& vertices){
	ModelStaticsItem itemVertex("Vertex Statistics");
	itemVertex.addStat("Total Vertex Count:", (int)vertices.size());
	modelStatics.push_back(itemVertex);
	}

void ModelGeneralStaticsCollector::fillPolygonsStatics(Model* model){
	std::unordered_map<int,int> verticesCount;

	for(vis::Polygon& polygon : model->getPolygons())
		verticesCount[model->getElementsRelations()->getVertexPolygonsById(polygon.getId()).size()]++;
		//verticesCount[polygon.getVertices().size()]++;

	ModelStaticsItem item("Polygon Statistics");
	typedef std::unordered_map<int,int>::const_iterator map_it;
	item.addStat("Total Polygons Count:", (int) model->getPolygons().size());
	for(map_it i = verticesCount.begin();i!=verticesCount.end();i++){
		std::stringstream ss;
		ss << i->first;
		ss << " vertices:";
		item.addStat(ss.str(),(int)i->second);
	}
	modelStatics.push_back(item);
}

void ModelGeneralStaticsCollector::fillPolyhedronsStatics(Model* model){
	std::unordered_map<int,int> facesCount;
	for(vis::Polyhedron& polyhedron : model->getPolyhedrons())
		facesCount[model->getElementsRelations()->getPolygonsPolyhedronsById(polyhedron.getId()).size()]++;

	ModelStaticsItem item("Polyhedron Statistics");
	item.addStat("Total Polyhedrons Count:",(int) model->getPolyhedrons().size());
	typedef std::unordered_map<int,int>::const_iterator map_it;
	for(map_it i = facesCount.begin();i!=facesCount.end();i++){
		std::stringstream ss;
		ss << i->first;
		ss << " faces:";
		item.addStat(ss.str(),(int)i->second);
	}
	modelStatics.push_back(item);
}
std::vector<ModelStaticsItem>& ModelGeneralStaticsCollector::getModelStatics(){
	return modelStatics;
}

ModelStaticsItem* ModelGeneralStaticsCollector::findItem(std::string title){
	typedef std::vector<ModelStaticsItem*>::size_type it_t;
	for(it_t i = 0;i<modelStatics.size();i++)
		if(title.compare(modelStatics[i].itemTitle)==0)
			return &modelStatics[i];
	return (ModelStaticsItem*)0;
}

void ModelGeneralStaticsCollector::clean(){
	modelStatics.clear();
}
