#ifndef MODEL_H
#define MODEL_H

#include "Common/Constants.h"
#include "Model/ElementsRelations/ElementsRelations.h"
#include <vector>
#include <string>
#include <memory>
#include <map>

namespace vis{
class Vertex;
class Edge;
class Polygon;
class Polyhedron;
}

/**
 * Internal representation of a 3D model
 *
 * This class keeps a container of objects for each element type available, as
 * a way to ensure memory locality inside each set. This containers are accesed by
 * other components through reference accessors to prevent copies of the internal
 * data. Usage of this containers requires preallocating data using reserve(), followed by
 * the forward construction of elements through emplace_back() instead of push_back().
 *
 * @warning Resizing a vector will result in the release of the original heap
 * allocation into a new region with a larger size. While the data will be moved/
 * copied to this new location, any pointer stored outside of this class will be
 * invalidated, as it will still point to the original section of memory.
 *
 * Currently the ModelLoading component is the only one that could trigger this
 * issue, so be careful when adding new file formats to the codebase, in
 * the case that a format doesn't specify the number of elements in its header (Ele/M3D),
 * the loader should store each element information temporarily while reading, followed
 * by the preallocation of the exact number of elements required.
 *
 * @note After the model is loaded, this containers are not modified until the
 * model is closed or until the program exits. Due to this its possible to replace
 * element pointers with their index for direct access. While this can reduce memory
 * consumption, be aware that a pointer to this model should be accessible inside
 * the method's own scope for this to work.
 */
class PropertyFieldDef;
class Model {
	public:
		Model(std::string filename);
		~Model();

		std::vector<float>& getBounds();
		std::string getFilename();

		/// Element container
		std::vector<vis::Vertex>& getVertices();
		std::vector<vis::Edge>& getAdditionalEdges();
		std::vector<vis::Polygon>& getPolygons();
		std::vector<vis::Polyhedron>& getPolyhedrons();

		/// Relations 
		ElementsRelations* getElementsRelations();

		/// Number of elements
		int getVerticesCount();
		int getAdditionalEdgesCount();
		int getPolygonsCount();
		int getPolyhedronsCount();

		void reserveVertices(int n);
		void reserveAdditionalEdges(int n);
		void reservePolygons(int n);
		void reservePolyhedrons(int n);

		/// Property field
		unsigned char getPropertyFieldPosition(PropertyFieldDef*);
		std::vector<std::shared_ptr<PropertyFieldDef>> &getPropertyFieldDefs();
		void addPropertyFieldDef(std::shared_ptr<PropertyFieldDef>);

		// Ele Node specific workaround (see below)
		bool is2D();
		void set2D(bool b);

		vis::Vertex* getVertex(unsigned int);

	private:
		std::string filename;
		std::vector<float> bounds; //{min x, min y, min z, max x, max y, max z}

		// Elements contained in the model
		std::vector<vis::Vertex> vertices;
		std::vector<vis::Edge> additionalEdges;
		std::vector<vis::Polygon> polygons;
		std::vector<vis::Polyhedron> polyhedrons;

		// Relations 
		ElementsRelations* relations;

		/**
		*  Property fields, the model keeps track of each field obtained through
		*  the PropertyFieldLoading component, in the case of properties obtained
		*  from evaluations, the data is stored on each strategy instead.
		*/
		std::vector<std::shared_ptr<PropertyFieldDef>> propertyFieldDefs;
		std::map<PropertyFieldDef*,unsigned char> propertyFieldPositions;

		/***
		*  In the case of the Ele/Node files, the two supported tools (TRIANGLE
		*  and Tetgen) will use the same format in different ways, this
		*  field is used by the Ele/Node exporter as a way to determine which
		*  specification was used previously.
		*
		*  @todo Allow the user to choose a specification for the export in the menu,
		*  thus removing the need to pass data between a ModeLoader and a ModelExport
		*  class through this field.
		*/
		bool _2D = false;
};

#endif // MODEL_H
