#ifndef MODELLOADINGTRI_H
#define MODELLOADINGTRI_H

#include "ModelLoading/ModelLoadingStrategy.h"
#include "Utils/streamscanner.h"
class ModelLoadingTri: public ModelLoadingStrategy{
	public:
		MODEL_LOADING_EXTENDING_CLASS_MINIMAL(ModelLoadingTri)
	protected:
	private:
		void readVertices(Model*);
		void readPolygons(Model*);
		StreamScanner parser;
	};

#endif // MODELLOADINGTRI_H
