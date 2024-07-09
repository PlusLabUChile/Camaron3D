#include "modelexportvisf.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Model.h"
#include "Rendering/RModel/rmodel.h"
#include "SelectionStrategies/Selection.h"
#include "ModelLoading/MeshProcessor.h"
#include "Utils/endianess.h"
ModelExportVisF::ModelExportVisF():
	ModelExportStrategy("VisF","visf")
{
}
ModelExportVisF::~ModelExportVisF(){

}

bool ModelExportVisF::exportModel(Model* m, std::string filename){
	if(m->getElementsRelations()->getPolygonsPolygons().size() == 0){
		MeshProcessor::completePolygonPolygonRelations(m);
	}
	std::ofstream outputFile(filename.c_str(), std::ios::trunc | std::ios::binary);

	int modelType = 0;
	if (m->getPolyhedronsCount() != 0){
		modelType = 2;
		}
	else if(m->getPolygonsCount() != 0){
		modelType = 1;
		}

	unsigned char endianness = Endianess::findEndianness();
	writeData(outputFile, endianness);
	writeData(outputFile, modelType);

	ModelExportVisF::exportVertices(m,outputFile);
	if (m->getPolygonsCount() != 0){
		ModelExportVisF::exportPolygons(m,outputFile);
		}

	if (m->getPolyhedronsCount() != 0){
		ModelExportVisF::exportPolyhedrons(m,outputFile);
		}
	outputFile.close();
	return true;

	}

bool ModelExportVisF::exportVertices(Model* m, std::ofstream& outputFile){
	std::vector<vis::Vertex>& vertices = m->getVertices();
	writeData(outputFile,(int)vertices.size());
	for( vis::Vertex& vertex : vertices ){
		writeData(outputFile,vertex.getCoords().x);
		writeData(outputFile,vertex.getCoords().y);
		writeData(outputFile,vertex.getCoords().z);
	}
	return true;
}

bool ModelExportVisF::exportPolygons(Model* m, std::ofstream& outputFile){
	std::vector<vis::Polygon>& polygons = m->getPolygons();
	writeData(outputFile,(int)polygons.size());
	for(vis::Polygon& polygon : polygons){
		// std::vector<vis::Vertex*>& currentPolygonVertices = polygon.getVertices();
		std::vector<unsigned int>& verticesPolIDS = m->getElementsRelations()->getVertexPolygonsById(polygon.getId());
		writeData(outputFile,(int)verticesPolIDS.size());
		for(unsigned int vertexID : verticesPolIDS)
			writeData(outputFile, m->getVertices()[vertexID].getId());
	}
	//saving neighbors
	writeData(outputFile,1); //exported with neighborhood info
	for( vis::Polygon& polygon : polygons){
		// std::vector<vis::Polygon*>& neighbors = polygon.getNeighborPolygons();
		std::vector<unsigned int>& neighborsPolIDS = m->getElementsRelations()->getPolygonsPolygonsById(polygon.getId());
		writeData(outputFile,(int)neighborsPolIDS.size());
		for( unsigned int neighborID : neighborsPolIDS)
			writeData(outputFile, neighborID);
	}
	return true;
}

bool ModelExportVisF::exportPolyhedrons(Model* m, std::ofstream& outputFile){
	std::vector<vis::Polyhedron>& polyhedrons = m->getPolyhedrons();
	writeData(outputFile,(int)polyhedrons.size());
	for( vis::Polyhedron& polyhedron : polyhedrons){
		// std::vector<vis::Polygon*>& currentPolygons = polyhedron.getPolyhedronPolygons();
		std::vector<unsigned int>& polygonIDS = m->getElementsRelations()->getPolygonsPolyhedronsById(polyhedron.getId());
		writeData(outputFile,(int)polygonIDS.size());
		for(unsigned int polygonID : polygonIDS)
			writeData(outputFile,polygonID);
	}
	return true;
}


bool ModelExportVisF::exportSelectedPolygons(Model* model, std::unordered_map<int,vis::Element*>& selectedElements, std::string filename){
	//TODO: ESTO NO ESTA FUNCIONANDO
	std::ofstream outputFile(filename.c_str());
	outputFile << "OFF" << std::endl;
	std::unordered_map<int,vis::Vertex*> exportedVertices;
	std::unordered_map<int,int> vertexIdVsVertexExportedPos;
	typedef std::unordered_map<int,vis::Element*>::const_iterator it_type;
	for(it_type it = selectedElements.begin();it!=selectedElements.end();it++){
		vis::Polygon* polygon = (vis::Polygon*)it->second;
		// std::vector<vis::Vertex*>& polygonVertices = polygon->getVertices();
		std::vector<unsigned int>& verticesPolygon = model->getElementsRelations()->getVertexPolygonsById(polygon->getId());
		for( unsigned int vertexID : verticesPolygon)
			exportedVertices[vertexID] = &model->getVertices()[vertexID];
	}
	outputFile << exportedVertices.size() << " " << selectedElements.size() << " 0" << std::endl;
	typedef std::unordered_map<int,vis::Vertex*>::const_iterator it_vertex_type;
	int vertexFilePos = 0;
	for(it_vertex_type it = exportedVertices.begin();it!=exportedVertices.end();it++){
		vis::Vertex* current = it->second;
		outputFile << current->getCoords().x << " ";
		outputFile << current->getCoords().y << " ";
		outputFile << current->getCoords().z << std::endl;
		vertexIdVsVertexExportedPos[current->getId()] = vertexFilePos;
		vertexFilePos++;
	}
	exportedVertices.clear();
	for(it_type it = selectedElements.begin();it!=selectedElements.end();it++){
		vis::Polygon* polygon = (vis::Polygon*)it->second;
		// std::vector<vis::Vertex*>& polygonVertices = polygon->getVertices();
		std::vector<unsigned int>& verticesPolygon = model->getElementsRelations()->getVertexPolygonsById(polygon->getId());
		outputFile << verticesPolygon.size();
		for( unsigned int vertexID : verticesPolygon)
			outputFile << " " << vertexIdVsVertexExportedPos[vertexID];
		outputFile << std::endl;
	}
	vertexIdVsVertexExportedPos.clear();
	outputFile << "#Exported with Vis from a Polygon Selection";
	outputFile.close();
	return true;
}


bool ModelExportVisF::exportSelectedPolyhedrons(Model* model, std::unordered_map<int,vis::Element*>& selectedElements,
												std::string filename){
	//TODO: ESTO NO ESTA FUNCIONANDO
	std::unordered_map<int,vis::Element*> selectedPolygons;
	typedef std::unordered_map<int,vis::Element*>::const_iterator it_type;
	for(it_type it = selectedElements.begin();it!=selectedElements.end();it++){
		vis::Polyhedron* polyhedron = (vis::Polyhedron*)it->second;
		//std::vector<vis::Polygon*>& polyhedronPolygons = polyhedron->getPolyhedronPolygons();
		std::vector<unsigned int>& polygosPolyhedronsID = model->getElementsRelations()->getPolygonsPolyhedronsById(polyhedron->getId());
		for(unsigned int polygonID : polygosPolyhedronsID){
			selectedPolygons[polygonID] = &model->getPolygons()[polygonID];
		}
	}
	return exportSelectedPolygons(model, selectedPolygons, filename);
}

bool ModelExportVisF::exportSelection(Selection* se, std::string filename){
	Model* model = se->getRModel()->getOriginalModel();
	if(se->getSelectionType() == vis::CONSTANTS::POLYGON)
		return exportSelectedPolygons(model, se->getSelectedElements(),filename);
	else if(se->getSelectionType() == vis::CONSTANTS::POLYHEDRON)
		return exportSelectedPolyhedrons(model, se->getSelectedElements(),filename);
	return false;
}

#include "Factories/modelexportstrategyregistry.h"
REGISTER_MODEL_EXPORT_STRATEGY(ModelExportVisF);
