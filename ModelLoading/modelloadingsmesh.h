#ifndef MODELLOADINGSMESH_H
#define MODELLOADINGSMESH_H
#include "ModelLoading/ModelLoadingStrategy.h"
#include "Utils/streamscanner.h"
#include <unordered_map>

class StreamScanner;
class ModelLoadingSmesh: public ModelLoadingStrategy
{
	public:
		MODEL_LOADING_EXTENDING_CLASS_MINIMAL(ModelLoadingSmesh)
	protected:
	private:
		void readHeaderSmesh(Model*);
		void readVerticesSmesh(Model*);
        void readVerticesNode(Model*);
		void readPolygons(Model*);
        void readHoles(Model*);
        void readRegion(Model*);

		// Smesh header
		int numberOfElements;
		int numberOfNodesPerElement;
		int numberOfAttributesPerElement;

		// Node List
		int numberOfNodes;
		int dimensions;
		int numberOfAttributesPerNode;
		int numberOfBoundaryMarkersNode;

        // Face list
        int numberOfFaces;
        int numberOfBoundaryMarkersFace;

        // Hole list;
        int numberOfHoles;

        // Region Attributes
        int numberOfRegion;
		std::vector<std::vector<float>> attributesNodes;
		
		StreamScanner parser;
		std::unordered_map<int,int> indexVsPosition;
		std::string path;
};

#endif // MODELLOADINGELENODE_H
