#ifndef MODELLOADINGSTRATEGY_H
#define MODELLOADINGSTRATEGY_H

#include <QThread>
#include "Exceptions/unknownextensionexception.h"
#include "Exceptions/modelloadingexception.h"
#include "UI/modelloadingprogressdialog.h"
#include <string>
#include <vector>

#define N_DEFAULT_THREADS_MODEL_LOADING 8
#define MODEL_LOAD_FUNC_DEC \
	virtual Model* load( std::string filename )
#define MODEL_VALIDATE_FUNC_DEC \
	virtual bool validate( std::string filename )
#define MODEL_LOADING_EXTENDING_CLASS_MINIMAL(x)\
	x();\
	virtual ~x();\
	MODEL_LOAD_FUNC_DEC;\
	MODEL_VALIDATE_FUNC_DEC;

class Model;
struct AcceptedFileFormat;

class ModelLoadingStrategy: public QThread {
		Q_OBJECT
	public:
		ModelLoadingStrategy();
		ModelLoadingStrategy(std::string fileFormatName, std::string fileFormatExt);
		virtual ~ModelLoadingStrategy();
		MODEL_LOAD_FUNC_DEC = 0;
		MODEL_VALIDATE_FUNC_DEC = 0;
		std::vector<AcceptedFileFormat>& getFileFormats();

		//QThread
		void run();
		void loadModelQThread(std::string filename);
		Model* getLoadedModel();

	signals:
		void modelLoadedSuccesfully();
		void setupProgressBarForNewModel(int,int,int);
		void setLoadedVertices(int);
		void setLoadedPolygons(int);
		void setLoadedPolyhedrons(int);
		void stageComplete(int);
		void addMessage(QString);
		void errorLoadingModel(QString);
		void warningLoadingModel(QString);

	protected:
		std::vector<AcceptedFileFormat> acceptedFileFormats;

		void updateBoundingBox(std::vector<float>& bounds, float x, float y, float z);
		void completeMesh(Model*);
		//void getEdgesAndPolygons(Model*, std::vector<unsigned int>&, unsigned int idPolygon);
		virtual void processPolygons(Model*);
		virtual void processPolyhedrons(Model*);

	private:
		Model* loadedModel;
		std::string modelPath;

};

#endif // MODELLOADINGSTRATEGY_H
