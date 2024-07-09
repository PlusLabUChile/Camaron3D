#ifndef MODELEXPORTVISF_H
#define MODELEXPORTVISF_H
#include "ModelExport/modelexportstrategy.h"
#include <unordered_map>
#include <fstream>
namespace vis {
	class Element;
}
class ModelExportVisF: public ModelExportStrategy
{
	public:
		ModelExportVisF();
		virtual ~ModelExportVisF();
		virtual bool exportModel(Model*, std::string filename);
		virtual bool exportSelection(Selection*, std::string filename);
	private:
		bool exportSelectedPolyhedrons(Model*, std::unordered_map<int,vis::Element*>& selectedElements,
									std::string filename);
		bool exportSelectedPolygons(Model*, std::unordered_map<int,vis::Element*>& selectedElements,
									std::string filename);

		bool exportVertices(Model*, std::ofstream& outputFile);
		bool exportPolygons(Model*, std::ofstream& outputFile);
		bool exportPolyhedrons(Model*, std::ofstream& outputFile);

		template<class T> inline void writeData(std::ofstream&, T val);
};
template<class T>
void ModelExportVisF::writeData(std::ofstream& file, T val){
	file.write((const char*)&val,sizeof(T));
}

#endif // MODELEXPORTVISF_H
