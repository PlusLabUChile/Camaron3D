#ifndef MODELLOADINGVISF_H
#define MODELLOADINGVISF_H
#include "ModelLoading/ModelLoadingStrategy.h"
#include "Utils/streamscanner.h"

class ModelLoadingVisF: public ModelLoadingStrategy {
	public:
		MODEL_LOADING_EXTENDING_CLASS_MINIMAL(ModelLoadingVisF)
	protected:
	private:
		void readHeader();
		void readBody(Model*);
		void readPolyhedrons(Model*);
		void readPolygons(Model*);
		void readVertices(Model*);

		void processPolygons(Model*) override;
		void processPolyhedrons(Model*) override;

		StreamScanner parser;
		std::string path;
	};

#endif // MODELLOADINGVISF_H
