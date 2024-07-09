#include "PropertyFieldLoading/PropertyFieldLoadingStrategy.h"
#include "Model/Model.h"
#include "Utils/fileutils.h"	//AcceptedFileFormat
#include "PropertyFieldLoading/propertyfielddef.h"
#include "Exceptions/unknownextensionexception.h"
#include "Exceptions/modelloadingexception.h"


PropertyFieldLoadingStrategy::PropertyFieldLoadingStrategy(): QThread(){
}
PropertyFieldLoadingStrategy::PropertyFieldLoadingStrategy(std::string fileFormatName,
										   std::string fileFormatExt): QThread()
{
	acceptedFileFormats.push_back(AcceptedFileFormat(fileFormatName,fileFormatExt));
}

PropertyFieldLoadingStrategy::~PropertyFieldLoadingStrategy()
{
	//dtor
}

std::vector<AcceptedFileFormat>& PropertyFieldLoadingStrategy::getFileFormats(){
	return acceptedFileFormats;
}

void PropertyFieldLoadingStrategy::run(){
	try{
		load(propertyFieldPath, model, this->selected);
		emit propertyFieldsLoadedSuccesfully();
	}catch(ExceptionMessage& e){
		emit errorLoadingPropertyField(QString::fromStdString(e.getMessage()));
	}catch(std::bad_alloc &ba){
		emit errorLoadingPropertyField( QString("Not enough RAM to load the model, try the x64 version"));
	}catch(std::exception& e){
		emit errorLoadingPropertyField( QString(e.what()));
	}
}
void PropertyFieldLoadingStrategy::loadPropertyFieldQThread(std::string propertyFieldPath, Model* model, std::vector<std::shared_ptr<PropertyFieldDef>> selected){
	this->propertyFieldPath = propertyFieldPath;
	this->model = model;
	this->selected = selected;
	this->start();
}
