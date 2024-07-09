#include "elementutils.h"
#include "Model/Element/Element.h"
#include "SelectionStrategies/Selection.h"
#include "Rendering/RModel/rmodel.h"
#include "Model/Model.h"
#include <iomanip>
#include <limits>
#include <sstream>
#include <map>
#include "Factories/EvaluationStrategyRegistry.h"
#include "EvaluationStrategies/EvaluationStrategy.h"
EvaluationStrategyRegistry* ElementUtils::instance =
		EvaluationStrategyRegistry::getSingletonInstance();

std::string ElementUtils::getPropertiesDisplay(vis::Element* elem, Selection* sel){
	std::stringstream ss;
	ss << std::setprecision(std::numeric_limits<float>::digits10+1);
	bool first = true;
	Model* model = sel->getRModel()->getOriginalModel();
	std::map<unsigned char, EvaluationStrategy*>* evaluationList = instance->getRegistryMap();
	for (std::map<unsigned char, EvaluationStrategy*>::const_iterator It = evaluationList->begin(); It != evaluationList->end(); ++It){
		if ((*(*It).second).hasBeenApplied()){
			if(!first){
				ss << "\n";
				}
			else {
				first = false;
				}

			ss << (*(*It).second).getPropertyName();
			std::vector<float>& evaluationData = (*(*It).second).getEvaluationResults(model);
			float position = elem->getId();
			ss << ": " << evaluationData[position];
			}
		}
	return ss.str();
}

ElementUtils::ElementUtils()
{
}
