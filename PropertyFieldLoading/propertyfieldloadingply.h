#ifndef PROPERTYFIELDLOADINGPLY_H
#define PROPERTYFIELDLOADINGPLY_H
#include "PropertyFieldLoading/PropertyFieldLoadingStrategy.h"
#include <vector>
#include <memory>
#include <sstream>

class Model;
class PropertyFieldLoadingPly: public PropertyFieldLoadingStrategy{
	public:
		PROPERTY_FIELD_LOADING_EXTENDING_CLASS_MINIMAL(PropertyFieldLoadingPly)

	private:
		bool readModelProperties( std::string, Model* , std::vector<std::shared_ptr<PropertyFieldDef>> );
		std::vector<float> readVertexProperties(std::ifstream&, int, std::vector<std::shared_ptr<PropertyFieldDef>>);

		std::string filename;
		std::vector<std::shared_ptr<PropertyFieldDef>> selectedProperties;
};

#endif // PROPERTYFIELDLOADINGPLY_H
