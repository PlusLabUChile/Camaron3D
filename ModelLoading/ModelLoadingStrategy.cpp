#include "ModelLoading/ModelLoadingStrategy.h"
#include "Model/Model.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Model.h"
#include "ModelLoading/MeshProcessor.h"
#include "Utils/fileutils.h"	//AcceptedFileFormat
#include "Utils/crosstimer.h"
#include <iostream>
ModelLoadingStrategy::ModelLoadingStrategy(): QThread(){
	loadedModel = 0;
	}

ModelLoadingStrategy::ModelLoadingStrategy(std::string fileFormatName,
										   std::string fileFormatExt): QThread(){
	loadedModel = 0;
	acceptedFileFormats.push_back(AcceptedFileFormat(fileFormatName,fileFormatExt));
	}

ModelLoadingStrategy::~ModelLoadingStrategy(){}

std::vector<AcceptedFileFormat>& ModelLoadingStrategy::getFileFormats(){
	return acceptedFileFormats;
	}

Model* ModelLoadingStrategy::getLoadedModel(){
	Model* temp = loadedModel;
	loadedModel = 0;
	return temp;
	}

void ModelLoadingStrategy::run(){
	try{
		loadedModel = 0;
		loadedModel = load(modelPath);
		if(loadedModel){	///@todo remove conditional if model cannot be 0
			completeMesh(loadedModel);
			emit stageComplete(ModelLoadingProgressDialog::BASE_MODE_READY);
			emit modelLoadedSuccesfully();
			}
		}
	catch(ExceptionMessage& e){
		emit errorLoadingModel(QString::fromStdString(e.getMessage()));
		}
	catch(std::bad_alloc &ba){
		emit errorLoadingModel( QString("Not enough RAM to load the model, try the x64 version"));
		}
	catch(std::exception& e){
		emit errorLoadingModel( QString(e.what()));
		}
	}

void ModelLoadingStrategy::loadModelQThread(std::string filename){
	modelPath = filename;
	this->start();
	}

void ModelLoadingStrategy::updateBoundingBox(std::vector<float> &bounds, float x, float y, float z){
	if(bounds[0] > x)
		bounds[0] = x;
	else if(bounds[3] < x)
		bounds[3] = x;
	if(bounds[1] > y)
		bounds[1] = y;
	else if(bounds[4] < y)
		bounds[4] = y;
	if( bounds[2] > z )
		bounds[2] = z;
	else if(bounds[5] < z)
		bounds[5] = z;
	}

void ModelLoadingStrategy::completeMesh(Model* mesh){
	if (mesh->getPolyhedronsCount() != 0){
		processPolyhedrons(mesh);
		}
	if (mesh->getPolygonsCount() != 0){
		processPolygons(mesh);
		}
	}

/*
void ModelLoadingStrategy::getEdgesAndPolygons(Model* mesh, std::vector<unsigned int>& vertices, unsigned int idPolygon){
	for(unsigned int j = 0; j < vertices.size(); j++){
		unsigned int v0, v1;
		if (j == vertices.size() - 1){
			v0 = vertices[0], v1 = vertices[j];
		}
		else{
			v0 = vertices[j], v1 = vertices[j+1];
		}
		if(v0 > v1) std::swap(v0,v1);
		std::pair<unsigned int, unsigned int> edge(v0,v1);
		auto it = mesh->getElementsRelations()->getEdgesPolygons().find(edge);
		if(it == mesh->getElementsRelations()->getEdgesPolygons().end()){
			mesh->getElementsRelations()->getEdgesPolygons().insert(std::make_pair(edge, std::make_pair(idPolygon, idPolygon)));
		}
		else{
			it->second.second = idPolygon;
			mesh->getElementsRelations()->getPolygonsPolygonsById(idPolygon).push_back(it->second.first);
			mesh->getElementsRelations()->getPolygonsPolygonsById(it->second.first).push_back(idPolygon);
		}
	}
}
*/
	
void ModelLoadingStrategy::processPolygons(Model* mesh){
	MeshProcessor::completeVertexPolygonRelations(mesh);
	emit stageComplete(ModelLoadingProgressDialog::COMPLETED_VERTEX_POLYGON_R);
	// MeshProcessor::completePolygonPolygonRelations(mesh);
	emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYGON_R);
	MeshProcessor::calculateNormalsPolygons(mesh);
	MeshProcessor::calculateNormalsVertices(mesh);
	emit stageComplete(ModelLoadingProgressDialog::NORMALS_CALCULATED);
	}

void ModelLoadingStrategy::processPolyhedrons(Model* mesh){
	//MeshProcessor::completePolygonPolyhedronRelations(mesh);
	emit stageComplete(ModelLoadingProgressDialog::COMPLETED_POLYGON_POLYHEDRON_R);
	MeshProcessor::calculateGeoCenterPolyhedronMesh(mesh);
	emit stageComplete(ModelLoadingProgressDialog::POLYHEDRON_GEOCENTER_CALCULATED);
	}
