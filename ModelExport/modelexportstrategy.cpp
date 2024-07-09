#include "modelexportstrategy.h"
#include "Model/Model.h"
#include "Utils/fileutils.h"
ModelExportStrategy::ModelExportStrategy(std::string n, std::string e):
	name(n), extension(e) {}

ModelExportStrategy::~ModelExportStrategy(){}

bool ModelExportStrategy::exportSelection(Selection*, std::string filename){
	throw ExceptionMessage("Can't export selection with extension: " +
						   FileUtils::getFileNameExtension(filename));
	}

bool ModelExportStrategy::exportModel(Model*, std::string filename){
	throw ExceptionMessage("Can't export model with extension: " +
						   FileUtils::getFileNameExtension(filename));
	}

std::string ModelExportStrategy::getName(){
	return name;
	}

std::string ModelExportStrategy::getExtension(){
	return extension;
	}
