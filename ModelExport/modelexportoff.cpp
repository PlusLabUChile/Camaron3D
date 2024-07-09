#include "modelexportoff.h"
#include "Model/Model.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "Rendering/RModel/rmodel.h"
#include "Common/Constants.h"
#include "SelectionStrategies/Selection.h"
#include <fstream>
ModelExportOFF::ModelExportOFF():
	ModelExportStrategy("OFF","off")
{
}
ModelExportOFF::~ModelExportOFF(){

}
bool ModelExportOFF::exportModel(Model* m, std::string filename){
	std::ofstream outputFile(filename.c_str(),std::ios::binary);
	outputFile << "OFF" << "\n";
	std::vector<vis::Vertex>& vertices = m->getVertices();
	std::vector<vis::Polygon>& polygons = m->getPolygons();
	outputFile << vertices.size() << " " << polygons.size() << " 0" << "\n";

	for( vis::Vertex& vertex : vertices){
		outputFile << vertex.getCoords().x << " ";
		outputFile << vertex.getCoords().y << " ";
		outputFile << vertex.getCoords().z << "\n";
	}
	for(vis::Polygon& polygon : polygons){
		std::vector<unsigned int>& currentPolygonVertices = m->getElementsRelations()->getVertexPolygonsById(polygon.getId());
		outputFile << currentPolygonVertices.size() << " ";
		for( unsigned int vID : currentPolygonVertices ){
			outputFile << m->getVertices()[vID].getId() << " ";

		}
		outputFile << "\n";
	}
	if (m->getPolyhedronsCount() != 0){
		outputFile << "#Exported with Vis from a PolyhedronMesh";
		}
	else {
		outputFile << "#Exported with Vis from a PolygonMesh";
		}

	outputFile.close();
	return true;
}

bool exportSelectedPolygons(Model* model, std::unordered_map<int,vis::Element*>& selectedElements, std::string filename){
	std::ofstream outputFile(filename.c_str());
	outputFile << "OFF" << "\n";
	std::unordered_map<int,vis::Vertex*> exportedVertices;
	std::unordered_map<int,int> vertexIdVsVertexExportedPos;
	typedef std::unordered_map<int,vis::Element*>::const_iterator it_type;
	for(it_type it = selectedElements.begin();it!=selectedElements.end();it++){
		vis::Polygon* polygon = (vis::Polygon*)it->second;
		// std::vector<vis::Vertex*>& polygonVertices = polygon->getVertices();
		std::vector<unsigned int>& verticesPolIDS = model->getElementsRelations()->getVertexPolygonsById(polygon->getId());
		for( unsigned int vertex : verticesPolIDS)
			exportedVertices[vertex] = &model->getVertices()[vertex];
	}
	outputFile << exportedVertices.size() << " " << selectedElements.size() << " 0" << "\n";
	typedef std::unordered_map<int,vis::Vertex*>::const_iterator it_vertex_type;
	int vertexFilePos = 0;
	for(it_vertex_type it = exportedVertices.begin();it!=exportedVertices.end();it++){
		vis::Vertex* current = it->second;
		outputFile << current->getCoords().x << " ";
		outputFile << current->getCoords().y << " ";
		outputFile << current->getCoords().z << "\n";
		vertexIdVsVertexExportedPos[current->getId()] = vertexFilePos;
		vertexFilePos++;
	}
	exportedVertices.clear();
	for(it_type it = selectedElements.begin();it!=selectedElements.end();it++){
		vis::Polygon* polygon = (vis::Polygon*)it->second;
		// std::vector<vis::Vertex*>& polygonVertices = polygon->getVertices();
		std::vector<unsigned int>& verticesPolIDS = model->getElementsRelations()->getVertexPolygonsById(polygon->getId());
		outputFile << verticesPolIDS.size();
		for( unsigned int vertex : verticesPolIDS)
			outputFile << " " << vertexIdVsVertexExportedPos[vertex];
		outputFile << "\n";
	}
	vertexIdVsVertexExportedPos.clear();
	outputFile << "#Exported with Vis from a Polygon Selection";
	outputFile.close();
	return true;
}

bool exportSelectedPolyhedrons(Model* model, std::unordered_map<int,vis::Element*>& selectedElements,
							std::string filename){
	std::unordered_map<int,vis::Element*> selectedPolygons;
	typedef std::unordered_map<int,vis::Element*>::const_iterator it_type;
	for(it_type it = selectedElements.begin();it!=selectedElements.end();it++){
		vis::Polyhedron* polyhedron = (vis::Polyhedron*)it->second;
		// std::vector<vis::Polygon*>& polyhedronPolygons = polyhedron->getPolyhedronPolygons();
		std::vector<unsigned int>& polygonIDS = model->getElementsRelations()->getPolygonsPolyhedronsById(polyhedron->getId());
		for( unsigned int polygon : polygonIDS){
			selectedPolygons[polygon] = &model->getPolygons()[polygon];
		}
	}
	return exportSelectedPolygons(model, selectedPolygons,filename);
}

bool ModelExportOFF::exportSelection(Selection* se, std::string filename){
	Model* model = se->getRModel()->getOriginalModel();
	if(se->getSelectionType() == vis::CONSTANTS::POLYGON)
		return exportSelectedPolygons(model, se->getSelectedElements(),filename);
	else if(se->getSelectionType() == vis::CONSTANTS::POLYHEDRON)
		return exportSelectedPolyhedrons(model, se->getSelectedElements(),filename);
	return false;
}

#include "Factories/modelexportstrategyregistry.h"
REGISTER_MODEL_EXPORT_STRATEGY(ModelExportOFF);
