#include "PropertyFieldLoading/propertyfieldloadingelenode.h"
#include "PropertyFieldLoading/scalarfielddef.h"
#include "Model/Model.h"
#include "Model/Element/Vertex.h"
#include "Utils/fileutils.h" //AcceptedFileFormat
#include "ModelLoading/modelloadingelenode.h"
#include <sstream>
#include <fstream>
#include <exception>
#include "Exceptions/unknownextensionexception.h"
#include "Exceptions/modelloadingexception.h"

PropertyFieldLoadingEleNode::PropertyFieldLoadingEleNode():PropertyFieldLoadingStrategy()
{
	acceptedFileFormats.push_back(AcceptedFileFormat("ELE","ele"));
	acceptedFileFormats.push_back(AcceptedFileFormat("ELE","node"));
}

PropertyFieldLoadingEleNode::~PropertyFieldLoadingEleNode(){}

bool PropertyFieldLoadingEleNode::validate(std::string filename){

	return ModelLoadingEleNode().validate(filename);
	}

std::vector<std::shared_ptr<PropertyFieldDef>> PropertyFieldLoadingEleNode::loadDefs(std::string filename) {
	std::vector<std::shared_ptr<PropertyFieldDef>> vertexProperties;
	std::string filenameNoExt = FileUtils::getFileNameWithoutExtension(filename);

	std::ifstream fileNode(filenameNoExt+".node");
	readHeaderNode(fileNode, vertexProperties);
	return vertexProperties;
}

void PropertyFieldLoadingEleNode::load(std::string filename,
								   Model* model,
								   std::vector<std::shared_ptr<PropertyFieldDef>> selectedProperties){
	try{
		this->filename = filename;
		this->selectedProperties = selectedProperties;
		readModelProperties(this->filename, model, this->selectedProperties);
		}
	catch(std::bad_alloc &ba){
		throw std::bad_alloc();
		}
	catch(ModelLoadingException &ex){
		throw ex;
		}
	}

void PropertyFieldLoadingEleNode::readHeaderNode(std::ifstream& file, std::vector<std::shared_ptr<PropertyFieldDef>>& vertexProperties){
	//scanner.reset(fileSizeNode);
	std::string line;
	FileUtils::safeGetline(file, line);
	std::istringstream iss(line);
	iss >> numberOfNodes;
	iss >> dimensions;
	iss >> numberOfAttributesPerNode;
	iss >> numberOfBoundaryMarkers;
	// The next properties start with property index = dimensions+1 because
	// the vertex index, X, Y and/or Z should also be counted in as previous indices.
	int propertyIndex = dimensions+1;
	for(int i=0;i< numberOfAttributesPerNode;i++) {
		std::ostringstream propName;
		propName << "Property #" << i;
		std::shared_ptr<ScalarFieldDef> prop
				= std::make_shared<ScalarFieldDef> (propertyIndex++,
													propName.str(),
													numberOfNodes);
		std::shared_ptr<PropertyFieldDef> scalarProp
				= std::dynamic_pointer_cast<PropertyFieldDef>(prop);
		vertexProperties.push_back(scalarProp);
	}
}

bool PropertyFieldLoadingEleNode::readModelProperties( std::string filename, Model* model, std::vector<std::shared_ptr<PropertyFieldDef>> selectedProperties) {
	std::vector<std::shared_ptr<ScalarFieldDef>> selectedScalars;
	for(std::shared_ptr<PropertyFieldDef> prop : selectedProperties) {
		selectedScalars.push_back(std::dynamic_pointer_cast<ScalarFieldDef>(prop));
	}

	std::string filenameNoExt = FileUtils::getFileNameWithoutExtension(filename);

	std::string filenameNode = filenameNoExt+".node";
	std::ifstream file(filenameNode);

	std::string line;
	// skip header
	FileUtils::safeGetline(file, line);
	auto totalProperties = selectedProperties.size();
	int totalProgress = 0;
	for(int i = 0; i < model->getVerticesCount(); i++){
		// Skip X,Y and possibly Z. First real property has id 0
		int currPropId = 0;
		FileUtils::safeGetline(file, line);
		std::istringstream iss(line);
		for(std::shared_ptr<ScalarFieldDef> prop : selectedScalars) {
			float value;
			while(currPropId <= prop->getId()){
				iss >> value;
				currPropId++;
			}
			std::vector<float>& content = prop->getPropertyData();
			content.push_back(value);
			prop->expandBounds(value);
		}
		totalProgress+=totalProperties;
		if(totalProgress%1000 == 0) {
			emit setLoadedProgress(totalProgress);
		}
	}
	for(std::shared_ptr<PropertyFieldDef> prop : selectedProperties) {
		model->addPropertyFieldDef(prop);
	}
	return true;
}

#include "Factories/PropertyFieldLoadingFactory.h"
REGISTER_PROPERTY_FIELD_LOADING_STRATEGY(PropertyFieldLoadingEleNode);
