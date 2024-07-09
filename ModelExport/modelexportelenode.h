#ifndef MODELEXPORTELENODE_H
#define MODELEXPORTELENODE_H

#include "ModelExport/modelexportstrategy.h"
class ModelExportEleNode: public ModelExportStrategy
{
	public:
		ModelExportEleNode();
		virtual ~ModelExportEleNode();
		virtual bool exportModel(Model*, std::string filename);
	private:
		void exportVertices(Model*,std::string filename);
};

#endif // MODELEXPORTELENODE_H
