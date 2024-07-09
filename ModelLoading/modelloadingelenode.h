#ifndef MODELLOADINGELENODE_H
#define MODELLOADINGELENODE_H
#include "ModelLoading/ModelLoadingStrategy.h"
#include "Utils/streamscanner.h"
#include <unordered_map>

class StreamScanner;
class ModelLoadingEleNode: public ModelLoadingStrategy
{
	public:
		MODEL_LOADING_EXTENDING_CLASS_MINIMAL(ModelLoadingEleNode)
	protected:
	private:
		void readHeaderNode(Model*);
		void readHeaderEle(Model*);
		void readVertices(Model*);

		// TRIANGLE
		void readPolygons(Model*);

		// TetGen
		void readPolyhedrons(Model*);
		void createPolygons(Model*, std::vector<int>& vertexComponents,
							std::vector<std::array<int, 4>>& isPartOfPolyhedron);

		//Ele header
		int numberOfElements;
		int numberOfNodesPerElement;
		int numberOfAttributesPerElement;

		//Node header
		int numberOfNodes;
		int dimensions;
		int numberOfAttributesPerNode;
		int numberOfBoundaryMarkers;

		StreamScanner parser;
		std::unordered_map<int,int> indexVsPosition;
		std::string path;

};

#endif // MODELLOADINGELENODE_H
