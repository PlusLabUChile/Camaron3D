#ifndef MESHEXTENSIONS_H
#define MESHEXTENSIONS_H

namespace vis{
class Vertex;
class Edge;
class Polygon;
class Polyhedron;
}
class Model;

class MeshExtensions {
	MeshExtensions();
	~MeshExtensions();
	public:
		static bool compare(vis::Vertex* first, vis::Vertex* second);
		static bool compare(vis::Edge* first, vis::Edge* second);
		static bool compare(vis::Polygon* first, vis::Polygon* second);
		static bool compare(vis::Polyhedron* first, vis::Polyhedron* second);
		static bool compare(Model* first, Model* second);

	private:

};

#endif