#ifndef MODELEXPORTOFF_H
#define MODELEXPORTOFF_H
#include "ModelExport/modelexportstrategy.h"

class ModelExportOFF: public ModelExportStrategy
{
	public:
		ModelExportOFF();
		virtual ~ModelExportOFF();
		virtual bool exportModel(Model*, std::string filename);
		virtual bool exportSelection(Selection*, std::string filename);
};

#endif // MODELEXPORTOFF_H
