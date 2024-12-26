#ifndef ELEMENTSRELATIONS_H
#define ELEMENTSRELATIONS_H

#include <vector>
#include <utility>
#include <map>
#include <cstddef>

namespace vis{
    class Vertex;
    class Edge;
    class Polygon;
    class Polyhedron;
}

/**
 * @brief Struct that store a vector of ids to be stored inside another vector
 */
struct relationsStruct{
    std::vector<unsigned int> v_element;

    std::vector<unsigned int>& getVectorIds() { 
        return v_element; 
    }
    
};

/**
 * @brief Struct that store a pair of ids to be saved in a vector
 */
struct relationPair{
    unsigned int first_value;
    unsigned int second_value;
    short int count;

    relationPair() : first_value(0), second_value(0), count(0) {};
    relationPair(unsigned int f, unsigned int s) : first_value(f), second_value(s), count(0) {};
};


/**
 * @brief Class that store information about a Model object.
 * 
 * This class saves different information about a Model loaded in Camaron.
 * This has 4 vectors of relationsStruct that store:
 *      - Vertices of a polygon.
 *      - Neighbors polygons.
 *      - Polygons that contain a vertex.
 *      - Polygons that represent a polyhedron.
 * 
 * Also it will save 2 vector of relationPair:
 *      - Relations about additional edges with his vertices.
 *      - Polyhedrons that a polygon belongs.
 */
class ElementsRelations{
    public:
        ElementsRelations();
        ~ElementsRelations();

        /**
         * Return the information about polygons that belongs each vertex of the model.
         */
        std::vector<relationsStruct>& getPolygonsVertex();

        /**
         * Return the information about neighbors polygon of each polygon.
         */
        std::vector<relationsStruct>& getPolygonsPolygons();

        /**
         * Return the information about vertices of each polygon.
         */
        std::vector<relationsStruct>& getVertexPolygons();

        /**
         * Return the information about polygons that represent a polyhedron.
         */
        std::vector<relationsStruct>& getPolygonsPolyhedrons();

        /**
         * Return the information about additional edges with his vertices.
         */
        std::vector<relationPair>& getVertexAdditionalEdges();

        /**
         * Return the information about polyhedrons that each polygon belongs.
         */
        std::vector<relationPair>& getPolyhedronPolygons();

        /**
         * Resize PolygonVertex container, creating instances of relationsStruct
         * @param int new size of PolygonVertex 
        */
        void resizePolygonsVertex(int);

        /**
         * Resize PolygonsPolygons container, creating instances of relationsStruct
         * @param int new size of PolygonsPolygons 
        */
        void resizePolygonsPolygons(int);

        /**
         * Resize VertexPolygons container, creating instances of relationsStruct
         * @param int new size of VertexPolygons 
        */
        void resizeVertexPolygons(int);

        /**
         * Resize PolygonsPolyhedrons container, creating instances of relationsStruct
         * @param int new size of PolygonsPolyhedrons 
        */
        void resizePolygonsPolyhedrons(int);

        /**
         * Resize VertexAdditionalEdges container, creating instances of relationsStruct
         * @param int new size of VertexAdditionalEdges 
        */
        void resizeVertexAdditionalEdges(int);
    
        /**
         * Resize PolyhedronPolygons container, creating instances of relationsStruct
         * @param int new size of PolyhedronPolygons 
        */
        void resizePolyhedronPolygons(int);

        /**
         * Return the information of polygons that the given vertex belongs.
         * @param id vertex identification number
         */
        std::vector<unsigned int>& getPolygonsVertexById(unsigned int id);

        /**
         * Return the information of neighbors polygons of a specific polygons.
         * @param id polygons identification number
         */
        std::vector<unsigned int>& getPolygonsPolygonsById(unsigned int id);

        /**
         * Return the information of polygons that the given vertex.
         * @param id polygon identification number
         */
        std::vector<unsigned int>& getVertexPolygonsById(unsigned int id);
    
        /**
         * Return the information of polygons that the given vertex.
         * @param id Polyhedron identification number
         */
        std::vector<unsigned int>& getPolygonsPolyhedronsById(unsigned int id);
    
        /**
         * Return the information of polygons that the given vertex.
         * @param id Edge identification number
         */
        relationPair& getVertexAdditionalEdgesById(unsigned int id);
    
        /**
         * Return the information of polyhedrons that the given vertex belongs.
         * @param id Polygons identification number
         */
        relationPair& getPolyhedronPolygonsById(unsigned int id);


        /**
         * Add the relation between a polyhedron and a polygon into PolyhedronPolygon
         * @param id_polyhedron Polyhedron identification number
         * @param id_polygon Polygon identification number
         */
        void addPolyhedronInPolygon(unsigned int id_polyhedron, unsigned id_polygon);

        /**
         * Add the relation between a polyhedron and a polygon into PolyhedronPolygon
         * @param id_polygon Polygon identification number
         * @param id_neighbor Polygon identification number
         */
        void addNeighborPolygonInPolygon(unsigned int id_polygon, unsigned int id_neighbor);

        /**
         * Add the relation between a vertex and a polygon into VertexPolygon
         * @param id_vertex Vertex identification number
         * @param id_polygon Polygon identification number
         */
        void addVertexInPolygon(unsigned int id_polygon, unsigned int id_vertex);

        /**
         * Add the relation between a vertex and a polygon into PolygonVertex
         * @param id_vertex Polyhedron identification number
         * @param id_polygon Polygon identification number
         */
        void addPolygonInVertex(unsigned int id_vertex, unsigned int id_polygon);

        /**
         * Add the relation between a polyhedron and a polygon into PolygonPolyhedron
         * @param id_polyhedron Polyhedron identification number
         * @param id_polygon Polygon identification number
         */
        void addPolygonInPolyhedron(unsigned int id_polyhedron, unsigned int id_polygon);

        /**
         * Add the relation between a polyhedron and a polygon into PolyhedronPolygon
         * @param id_polyhedron Polyhedron identification number
         * @param id_polygon Polygon identification number
         */
        void addPolygonPolyhedronRelations(unsigned int, unsigned int);

        /**
         * Store the relation between the position of a vertex inside model's vertices container and his id.
         * @param pos position in the container.
         * @param id identification number of the vertex. 
        */
        void addIdAndPositionInContainerVertex(unsigned int, unsigned int);

        /**
         * Return the position of a vertex in the Model's Vertices container with his id
         * @param uint vertex's id.
         */
        unsigned int getPositionInContainerById(unsigned int);

        /**
         * Check if the ids of each model's vertices are equal to their position in vertices's container
         */
        void checkVertices();


        void setDiffVertex(bool);
        void setDiffPolygon(bool);
        void setDiffPolyhedron(bool);

        bool getDiffVertex();
        bool getDiffPolygon();
        bool getDiffPolyhedron();

    protected:
    private:
        // Containers
        std::vector<relationsStruct> PolygonsVertex;
        std::vector<relationsStruct> PolygonsPolygons;
        std::vector<relationsStruct> VertexPolygons;
        std::vector<relationsStruct> PolygonsPolyhedrons;
        std::vector<relationPair> VertexAdditionalEdges;
        std::vector<relationPair> PolyhedronPolygons;

        /*
        * Container that has the relations between index vertex and his position in Model's Vertice vector.
        * it's used in Polyhedron meshes loaded by ele/node files.
        */
        std::map<unsigned int, unsigned int> vertexIndexInPosition;
        bool diffVertex;
        bool diffPolygon;
        bool diffPolyhedron;
};

#endif // ELEMENTSRELATIONS_H
