#ifndef EVALUATIONSTRATEGY_H
#define EVALUATIONSTRATEGY_H

#include <string>
#include <vector>
#include <QWidget>
#define VERTEX_CRITERIA_WEIGHT_BASE 1.0f
#define POLYGON_CRITERIA_WEIGHT_BASE 100.0f
#define TRIANGLE_CRITERIA_WEIGHT_BASE 150.0f
#define POLYHEDRON_CRITERIA_WEIGHT_BASE 200.0f
namespace vis{
class Vertex;
class Polyhedron;
class Polygon;
class Edge;
}
class Model;
class EvaluationStrategy
{
	public:
		EvaluationStrategy(std::string newname,
						   std::string newPropertyName,
						   unsigned char idn = 0);
		EvaluationStrategy(std::string newname,
						   std::string newdescription,
						   std::string newPropertyName,
						   unsigned char idn = 0);
		virtual ~EvaluationStrategy();
		void setId(unsigned char idn);
		std::string getDescription();
		virtual void value(Model* model, vis::Vertex* v);
		virtual void value(Model* model, vis::Polygon* m);
		virtual void value(Model* model, vis::Polyhedron* m);
		virtual void value(Model* model, vis::Edge* m);
		const char* getName();
		const char* getPropertyName();
		float getWeight();
		void setWeight(float);

		virtual void setup(Model*) = 0;
		virtual void evaluateElementsFrom(Model* model) = 0;
		virtual void requestEvaluationResultsAsRModel(Model* model, std::vector<float>& target) = 0;
		virtual int getSupportedType() = 0;
		std::vector<float>& getEvaluationResults(Model* model);

		//statistics
		void setTotalElementsCount(int);
		int getTotalElementsCount();
		int getCalculatedValuesCount();
		int getValuesCountInRange(float from, float to);
		float getMinValue();
		float getMaxValue();
		bool hasStatics();
		bool hasBeenApplied();
		//could not be applied
		virtual float getNullValue() = 0;
		//Description QWidget
		virtual bool hasDescriptionQWidget();
		virtual QWidget* getEvaluationStrategyDescriptionQWidget();
		virtual void QApplicationInitiatedEv();
		bool hasQIcon();
		QIcon* getEvaluationStrategyQIcon();

	protected:
		const std::string description;
		const std::string name;
		const std::string propertyName;
		void addNewValue(float);
		unsigned char id;
		bool saveStatics;
		bool applied;
		std::vector<float> values;
		std::vector<float> sortedValues;
		float minValue;
		float maxValue;
		int totalElementsCount;
		bool valuesNeedSorting;
		float weight;
		QIcon* qicon;

		template<class ELEMENT>
		void applyEvaluation(Model* model, std::vector<ELEMENT>& content);

		template<class ELEMENT>
		void mapResultsToRModel(std::vector<ELEMENT>& content, std::vector<float>& target);

	private:
		void initVariables(int idn);
};

template<class ELEMENT>
void EvaluationStrategy::applyEvaluation(Model* model, std::vector<ELEMENT>& content){
	for(ELEMENT& ele : content){
		value(model, &ele);
		}
	}

template<class ELEMENT>
void EvaluationStrategy::mapResultsToRModel(std::vector<ELEMENT>& content, std::vector<float>& target){
	for(unsigned int i = 0; i < content.size(); i++){
		std::vector<int>& rmodelPositions = content[i].getRmodelPositions();
		for(int rmodelPosition : rmodelPositions){
			target[rmodelPosition] = this->values[i];
			}
		}
	}


#endif // EVALUATIONSTRATEGY_H
