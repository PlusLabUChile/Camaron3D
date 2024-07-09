#include "unitcube.h"
#include "Model/Model.h"
#include "Model/Element/Edge.h"
#include "Model/Element/Vertex.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"

UnitCube::UnitCube(bool tri){
	triangulate = tri;

	createVertexCloud();
	createPolygonMesh();
	createPolyhedronMesh();
}

UnitCube::~UnitCube(){}

void UnitCube::loadVertices(Model* model){
	float verts[8][3] = {{0.0f, 0.0f, 0.0f},
						 {0.0f, 0.0f, 1.0f},
						 {0.0f, 1.0f, 0.0f},
						 {0.0f, 1.0f, 1.0f},
						 {1.0f, 0.0f, 0.0f},
						 {1.0f, 0.0f, 1.0f},
						 {1.0f, 1.0f, 0.0f},
						 {1.0f, 1.0f, 1.0f}};

	model->reserveVertices(8);
	std::vector<vis::Vertex>& vertices = model->getVertices();
	for (int i = 0; i < 8; i++){
		vertices.emplace_back(i, verts[i][0], verts[i][1], verts[i][2]);
		}
	}

void UnitCube::loadEdges(Model* model){
	int edg[4][5] = {{1, 6, 150, 0, 0},
					 {1, 7, 0, 150, 0},
					 {1, 4, 0, 0, 150},
					 {1, 3, 150, 150, 0}};

	std::vector<vis::Vertex>& vertices = model->getVertices();

	model->reserveAdditionalEdges(4);
	std::vector<vis::Edge>& edges = model->getAdditionalEdges();
	for (int i = 0; i < 4; i++){
		edges.emplace_back(i, &vertices[edg[i][0]], &vertices[edg[i][1]],
					    glm::vec3(edg[i][2]/255.0f,edg[i][3]/255.0f,
						edg[i][4]/255.0f));
		}
	}

void UnitCube::loadTriangles(Model* model){
	int ids[18][3] = {{0, 6, 7}, {6, 7, 4},
					  {7, 4, 0}, {4, 0, 6},
					  {4, 7, 5}, {7, 5, 0},
					  {5, 0, 4}, {0, 2, 7},
					  {2, 7, 6}, {6, 0, 2},
					  {0, 3, 7}, {3, 7, 2},
					  {2, 0, 3}, {5, 7, 1},
					  {7, 1, 0}, {1, 0, 5},
					  {1, 7, 3}, {3, 0, 1}};

	model->reservePolygons(18);
	std::vector<vis::Polygon>& triangles = model->getPolygons();
	std::vector<vis::Vertex>& vertices = model->getVertices();
	for (int i = 0; i < 18; i++){
		triangles.emplace_back(i);
		vis::Polygon* triangle = &triangles[i];
		std::vector<vis::Vertex*>& verts = triangle->getVertices();
		verts.reserve(3);
		for (int id : ids[i]){
			verts.push_back(&vertices[id]);
			}
		}
	}

void UnitCube::loadSquares(Model* model){
	int ids[6][4] = {{0, 2, 6, 4},
					 {4, 6, 7, 5},
					 {0, 4, 5, 1},
					 {1, 3, 2, 0},
					 {3, 7, 6, 2},
					 {1, 5, 7, 3}};

	model->reservePolygons(6);
	std::vector<vis::Polygon>& squares = model->getPolygons();
	std::vector<vis::Vertex>& vertices = model->getVertices();

	for (int i = 0; i < 6; i++){
		squares.emplace_back(i);
		vis::Polygon* square = &squares[i];
		std::vector<vis::Vertex*>& verts = square->getVertices();
		verts.reserve(4);
		for (int id : ids[i]){
			verts.push_back(&vertices[id]);
			}
		}
	}

void UnitCube::loadTetrahedrons(Model* model){
	int ids[6][4] = {{0, 1, 2, 3},
					 {2, 4, 5, 6},
					 {7, 8, 0, 9},
					 {10, 11, 7, 12},
					 {5, 13, 14, 15},
					 {14, 16, 10, 17}};

	model->reservePolyhedrons(6);
	std::vector<vis::Polyhedron>& tetrahedrons = model->getPolyhedrons();
	std::vector<vis::Polygon>& triangles = model->getPolygons();
	for (int i = 0; i < 6; i++){
		tetrahedrons.emplace_back(i);
		vis::Polyhedron* tetra = &tetrahedrons[i];
		std::vector<vis::Polygon*> &polygons = tetra->getPolyhedronPolygons();
		polygons.reserve(4);
		for (int id : ids[i]){
			polygons.push_back(&triangles[id]);
			}
		}
	}

void UnitCube::loadHexahedron(Model* model){
	int ids[6] = {0, 1, 2, 3, 4, 5};
	
	model->reservePolyhedrons(1);
	std::vector<vis::Polyhedron>& hexahedron = model->getPolyhedrons();
	std::vector<vis::Polygon>& squares = model->getPolygons();

	hexahedron.emplace_back(0);
	vis::Polyhedron* cube = &hexahedron[0];
	std::vector<vis::Polygon*> &polygons = cube->getPolyhedronPolygons();
	polygons.reserve(6);
	for (int id : ids){
		polygons.push_back(&squares[id]);
		}
	}

void UnitCube::createVertexCloud(){
	vertexcloud = new Model("");
	loadVertices(vertexcloud);
	setBounds(vertexcloud);

}

void UnitCube::createPolygonMesh(){
	polygonmesh = new Model("");
	loadVertices(polygonmesh);
	if(triangulate){
		loadTriangles(polygonmesh);
		}
	else{
		loadSquares(polygonmesh);
		}

	setBounds(polygonmesh);
}

void UnitCube::createPolyhedronMesh(){
	polyhedronmesh = new Model("");
	loadVertices(polyhedronmesh);
	if (triangulate){
		loadTriangles(polyhedronmesh);
		loadTetrahedrons(polyhedronmesh);
		}
	else{
		loadSquares(polyhedronmesh);
		loadHexahedron(polyhedronmesh);
		}
	setBounds(polyhedronmesh);
}

void UnitCube::setBounds(Model* mesh){
	std::vector<float> &bounds = mesh->getBounds();
	bounds.resize(6);
	bounds[0] = bounds[1] = bounds[2] = 0.0f;
	bounds[3] = bounds[4] = bounds[5] = 1.0f;
	}

void UnitCube::setAdditionalEdges(){
	loadEdges(vertexcloud);
	loadEdges(polygonmesh);
	loadEdges(polyhedronmesh);
	}

