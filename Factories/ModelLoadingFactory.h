#ifndef MODELLOADINGFACTORY_H
#define MODELLOADINGFACTORY_H
#include <vector>
#include "RegistryTemplate.h"

#define REGISTER_MODEL_LOADING_STRATEGY(x) \
	int x##_dummy_var  = \
	ModelLoadingFactory::getSingletonInstance()->\
	addNewModelLoadingStrategy(new x())

class ModelLoadingStrategy;
class Model;
class ModelLoadingFactory: public RegistryTemplate<std::string,ModelLoadingStrategy> {
	public:
		ModelLoadingFactory();
		virtual ~ModelLoadingFactory();
		static ModelLoadingFactory* getSingletonInstance();
		static void deleteSingletonInstances();
		
		std::vector<ModelLoadingStrategy*>& getModelLoadingStrategies();
		bool addNewModelLoadingStrategy( ModelLoadingStrategy*);
		ModelLoadingStrategy* loadModelQThread( std::string );
	protected:
	private:
		static ModelLoadingFactory* instance;
		std::vector<ModelLoadingStrategy*> modelLoadingStrategies;
};

#endif // MODELLOADINGFACTORY_H
