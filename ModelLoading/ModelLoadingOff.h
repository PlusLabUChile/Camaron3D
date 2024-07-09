#ifndef MODELLOADINGOFF_H
#define MODELLOADINGOFF_H
#include "Utils/streamscanner.h"
#include "ModelLoading/ModelLoadingStrategy.h"
class Model;

class ModelLoadingOff: public ModelLoadingStrategy{
	public:
		MODEL_LOADING_EXTENDING_CLASS_MINIMAL(ModelLoadingOff)
	protected:
	private:
		void readHeader(Model*);
		void readVertices(Model*);
		void readPolygons(Model*);
		StreamScanner parser;
	};

#endif // MODELLOADINGOFF_H
