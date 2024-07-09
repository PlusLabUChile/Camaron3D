#include "Factories/ModelLoadingFactory.h"
#include "ModelLoading/ModelLoadingStrategy.h"
#include "Utils/fileutils.h"
#include "Exceptions/modelloadingexception.h"
#include <iostream>
ModelLoadingFactory* ModelLoadingFactory::instance = (ModelLoadingFactory*)0;
ModelLoadingFactory::ModelLoadingFactory(): RegistryTemplate<std::string, ModelLoadingStrategy>()
{
}

ModelLoadingFactory::~ModelLoadingFactory()
{
	for( ModelLoadingStrategy* strategy : modelLoadingStrategies ) {
#ifdef DEBUG_MOD
		std::cout << "Deleting Model Loading Strategy from Factory: ";
		std::cout << strategy->getFileFormats()[0].fileFormatName << std::endl;
#endif
		delete strategy;
	}
}
bool ModelLoadingFactory::addNewModelLoadingStrategy( ModelLoadingStrategy* ml){
	bool added = false;
	for( AcceptedFileFormat& fileFormat : ml->getFileFormats() ){
		this->registerInstance(fileFormat.fileFormatExt,ml);
		added = true;
	}
	if(added)
		modelLoadingStrategies.push_back(ml);
	else
		delete ml;
	return added;

}

ModelLoadingStrategy* ModelLoadingFactory::loadModelQThread( std::string filename ){
	if(!FileUtils::fileExists(filename)){
		throw ExceptionMessage("File not found:<br>"+filename);
	}
	std::string ext = FileUtils::getFileNameExtension(filename);

#ifdef DEBUG_MOD
	std::cout << "File: " << filename << ", Extension: " << ext << "\n";
#endif
	ModelLoadingStrategy* strategy = this->getRegistryByKeyInstance( ext );
	if( strategy ) {
		if(strategy->validate( filename )){ ///@todo Should catch exception here
			return strategy;
		}
	}
	throw UnknownExtensionException(filename);
}
std::vector<ModelLoadingStrategy*>& ModelLoadingFactory::getModelLoadingStrategies(){
	return modelLoadingStrategies;
}

ModelLoadingFactory* ModelLoadingFactory::getSingletonInstance(){
	if(!ModelLoadingFactory::instance)
		ModelLoadingFactory::instance = new ModelLoadingFactory();
	return ModelLoadingFactory::instance;
}

void ModelLoadingFactory::deleteSingletonInstances(){
	if(ModelLoadingFactory::instance)
		delete ModelLoadingFactory::instance;
}

