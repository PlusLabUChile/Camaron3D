#include "EvaluationStrategies/EvaluationStrategy.h"
#include "Utils/sortsearchutils.h"
#include "Model/Model.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polyhedron.h"
#include <QIcon>


EvaluationStrategy::EvaluationStrategy(std::string newname, std::string newdescription,
									   std::string newPropertyName,
									   unsigned char idn):
	description(newdescription),
	name(newname),
	propertyName(newPropertyName)
{
	initVariables(idn);
}

EvaluationStrategy::EvaluationStrategy(std::string newname,
				   std::string newPropertyName,
				   unsigned char idn):
	description("No description\navailable."),
	name(newname),
	propertyName(newPropertyName)
{
	initVariables(idn);
}

void EvaluationStrategy::initVariables(int idn){
	this->id = idn;
	qicon = 0;
	this->applied = false;
	this->valuesNeedSorting = false;
	this->totalElementsCount = 0;
	this->minValue = std::numeric_limits<float>::max();
	this->maxValue = -std::numeric_limits<float>::max();
	saveStatics = true;
	weight = VERTEX_CRITERIA_WEIGHT_BASE;
}

EvaluationStrategy::~EvaluationStrategy()
{
	if(qicon)
		delete qicon;
}
void EvaluationStrategy::value(Model* model, vis::Vertex*){}
void EvaluationStrategy::value(Model* model, vis::Polygon*){}
void EvaluationStrategy::value(Model* model, vis::Polyhedron*){}
void EvaluationStrategy::value(Model* model, vis::Edge*){}
const char* EvaluationStrategy::getName( ){
	return this->name.c_str();
}
const char* EvaluationStrategy::getPropertyName(){
	return this->propertyName.c_str();
}
void EvaluationStrategy::setTotalElementsCount(int n){
	if(!saveStatics)
		return;
	this->totalElementsCount = n;
	this->sortedValues.clear();
	this->values.clear();
	this->applied = false;
	if(n!=0){
		this->sortedValues.reserve(n);
		this->values.reserve(n);
		}
	else{
		std::vector<float>().swap(sortedValues);
		std::vector<float>().swap(values);
		}
	this->minValue = std::numeric_limits<float>::max();
	this->maxValue = -std::numeric_limits<float>::max();
}
int EvaluationStrategy::getTotalElementsCount(){
	return this->totalElementsCount;
}
void EvaluationStrategy::addNewValue(float val){
	this->sortedValues.push_back(val);
	this->values.push_back(val);
	this->minValue = std::min(val,this->minValue);
	this->maxValue = std::max(val,this->maxValue);
	this->valuesNeedSorting = true;
}
int EvaluationStrategy::getCalculatedValuesCount(){
	return this->sortedValues.size();
}
int EvaluationStrategy::getValuesCountInRange(float from, float to){
	if(from>to)
		return 0;
	if(sortedValues.size()==0)
		return 0;
	if(this->valuesNeedSorting){
		this->valuesNeedSorting = false;
		std::sort(sortedValues.begin(),sortedValues.end());
	}
	int posFrom = SortSearchUtils::binarySearchPosition(sortedValues,from);
	int posTo = SortSearchUtils::binarySearchPosition(sortedValues,to);
	return posTo-posFrom;
}
float EvaluationStrategy::getMinValue(){
	return minValue;
}

float EvaluationStrategy::getMaxValue(){
	return maxValue;
}

void EvaluationStrategy::setId(unsigned char idn){
	id = idn;
}

std::vector<float>& EvaluationStrategy::getEvaluationResults(Model* model){
	if(!applied){
		evaluateElementsFrom(model);
		}

	return this->values;
	}

bool EvaluationStrategy::hasStatics(){
	return saveStatics;
}

bool EvaluationStrategy::hasBeenApplied(){
	return applied;
	}

float EvaluationStrategy::getWeight(){
	return weight;
}
void EvaluationStrategy::setWeight(float f){
	weight = f;
}
std::string EvaluationStrategy::getDescription(){
	return description;
}
bool EvaluationStrategy::hasDescriptionQWidget(){
	return false;
}

QWidget* EvaluationStrategy::getEvaluationStrategyDescriptionQWidget(){
	return (QWidget*)0;
}
void EvaluationStrategy::QApplicationInitiatedEv(){}

bool EvaluationStrategy::hasQIcon(){
	return qicon;
}

QIcon* EvaluationStrategy::getEvaluationStrategyQIcon(){
	return qicon;
}
