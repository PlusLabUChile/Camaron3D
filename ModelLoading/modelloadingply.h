#ifndef MODELLOADINGPLY_H
#define MODELLOADINGPLY_H
#include "ModelLoading/ModelLoadingStrategy.h"
#include "Utils/streamscanner.h"
#include <memory>
class Model;
class PropertyFieldDef;

class ModelLoadingPly: public ModelLoadingStrategy {
	public:
		// Macro definition for validate and load methods
		MODEL_LOADING_EXTENDING_CLASS_MINIMAL(ModelLoadingPly)
	protected:
	private:
		void readHeader(Model*);
		void readPolygons(Model*);
		void readVertices(Model*);
		void readAdditionalEdges(Model*);
		int numberOfBytesInVertexPropertiesToIgnore;
		std::vector<std::shared_ptr<PropertyFieldDef>> vertexProperties;
		StreamScanner parser;
	};

#endif // MODELLOADINGPLY_H
