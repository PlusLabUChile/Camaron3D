#include "modelexportelenode.h"
#include "Model/Model.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "Utils/fileutils.h"
#include "Utils/polyhedronutils.h"
#include <fstream>
ModelExportEleNode::ModelExportEleNode():
	ModelExportStrategy("Node, Ele","ele"){}

ModelExportEleNode::~ModelExportEleNode(){}

bool ModelExportEleNode::exportModel(Model* m, std::string filename){
	std::string filenameNode = FileUtils::getFileNameWithoutExtension(filename)+".node";
	std::string filenameEle = FileUtils::getFileNameWithoutExtension(filename)+".ele";
	exportVertices(m,filenameNode);
	std::ofstream outputFile(filenameEle.c_str());

	ElementsRelations* relations = relations;

	//TetGen model
	if (m->getPolyhedronsCount() != 0){
		std::vector<vis::Polyhedron>& polyhedrons = m->getPolyhedrons();
		int numberOfTetrahedrons = 0;
		for( vis::Polyhedron& polyhedron : polyhedrons){
			if(relations->getPolygonsPolyhedronsById(polyhedron.getId()).size() == 4){
				numberOfTetrahedrons++;
				}
			}
		outputFile << numberOfTetrahedrons << " 4 0" << std::endl;
		for( vis::Polyhedron& polyhedron : polyhedrons ){
			if(relations->getPolygonsPolyhedronsById(polyhedron.getId()).size() != 4)
				continue;
			std::vector<unsigned int> currentTetraVerticesIDS;
			PolyhedronUtils::getPolyhedronVertices(m, &polyhedron, currentTetraVerticesIDS);
			if(currentTetraVerticesIDS.size() !=4 )
				continue;
			outputFile << polyhedron.getId() << " ";
			outputFile << currentTetraVerticesIDS[0] << " ";
			outputFile << currentTetraVerticesIDS[1] << " ";
			outputFile << currentTetraVerticesIDS[2] << " ";
			outputFile << currentTetraVerticesIDS[3] << std::endl;
			}
		outputFile << "#Exported with Vis from a PolyhedronMesh";
		}

	//TRIANGLE model
	else {
		std::vector<vis::Polygon>& polygons = m->getPolygons();
		int numberOfTriangles = 0;
		for( vis::Polygon& polygon : polygons )
			numberOfTriangles += (relations->getVertexPolygonsById(polygon.getId()).size() - 2);
		outputFile << numberOfTriangles << " 3 0" << std::endl;

		int currentTriangleId = 1;
		for( vis::Polygon& polygon : polygons){
			std::vector<unsigned int>& currentPolygonVertices = relations->getVertexPolygonsById(polygon.getId());
			for( decltype(currentPolygonVertices.size()) j = 1; j < currentPolygonVertices.size() - 1; j++ ) {
				outputFile << currentTriangleId++ << " ";
				//outputFile << polygon->getId() << " ";
				outputFile << currentPolygonVertices[0] << " ";
				outputFile << currentPolygonVertices[j] << " ";
				outputFile << currentPolygonVertices[j+1] << std::endl;
				}
			}
		outputFile << "#Exported with Vis from a PolygonMesh";
		}

	outputFile.close();
	return true;
	}


void ModelExportEleNode::exportVertices(Model* m,std::string filename){
	std::ofstream outputFile(filename.c_str());
	std::vector<vis::Vertex>& vertices = m->getVertices();
	outputFile << vertices.size() << " " << ((m->is2D())?2:3) << " 0 0" << std::endl;
	for( vis::Vertex& vertex : vertices){
		outputFile << vertex.getId() << " ";
		outputFile << vertex.getCoords().x << " ";
		if(m->is2D())
			outputFile << vertex.getCoords().y << std::endl;
		else{
			outputFile << vertex.getCoords().y << " ";
			outputFile << vertex.getCoords().z << std::endl;
		}
	}
	outputFile << "#Exported with vis";
	outputFile.close();
}

#include "Factories/modelexportstrategyregistry.h"
REGISTER_MODEL_EXPORT_STRATEGY(ModelExportEleNode);
