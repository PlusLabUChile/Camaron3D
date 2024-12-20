#include "Model/ElementsRelations/ElementsRelations.h"
#include <iostream>

ElementsRelations::ElementsRelations() :
    diffVertex(false),
    diffPolygon(false),
    diffPolyhedron(false)
{}

ElementsRelations::~ElementsRelations(){}

std::vector<relationsStruct>& ElementsRelations::getPolygonsVertex(){
    return PolygonsVertex;
}

std::vector<relationsStruct>& ElementsRelations::getPolygonsPolygons(){
    return PolygonsPolygons;
}

std::vector<relationsStruct>& ElementsRelations::getVertexPolygons(){
    return VertexPolygons;
}

std::vector<relationsStruct>& ElementsRelations::getPolygonsPolyhedrons(){
    return PolygonsPolyhedrons;
}

std::vector<relationPair>& ElementsRelations::getVertexAdditionalEdges(){
    return VertexAdditionalEdges;
}

std::vector<relationPair>& ElementsRelations::getPolyhedronPolygons(){
    return PolyhedronPolygons;
}


void ElementsRelations::resizePolygonsVertex(int n){
    PolygonsVertex.resize(n);
}

void ElementsRelations::resizePolygonsPolygons(int n){
    PolygonsPolygons.resize(n);
}

void ElementsRelations::resizeVertexPolygons(int n){
    VertexPolygons.resize(n);
}

void ElementsRelations::resizePolygonsPolyhedrons(int n){
    PolygonsPolyhedrons.resize(n);
}

void ElementsRelations::resizeVertexAdditionalEdges(int n){
    VertexAdditionalEdges.resize(n);
}

void ElementsRelations::resizePolyhedronPolygons(int n){
    PolyhedronPolygons.resize(n);
}


std::vector<unsigned int>& ElementsRelations::getPolygonsVertexById(unsigned int id){
    if(diffVertex) id--;
    return PolygonsVertex[id].getVectorIds();
}

std::vector<unsigned int>& ElementsRelations::getPolygonsPolygonsById(unsigned int id){
    if(diffPolygon) id--;
    return PolygonsPolygons[id].getVectorIds();
}

std::vector<unsigned int>& ElementsRelations::getVertexPolygonsById(unsigned int id){
    if(diffPolygon) id--;
    return VertexPolygons[id].getVectorIds();
}

std::vector<unsigned int>& ElementsRelations::getPolygonsPolyhedronsById(unsigned int id){
    if(diffPolyhedron) id--;
    return PolygonsPolyhedrons[id].getVectorIds();
}

relationPair& ElementsRelations::getVertexAdditionalEdgesById(unsigned int id){
    return VertexAdditionalEdges[id];
}

relationPair& ElementsRelations::getPolyhedronPolygonsById(unsigned int id){
    if(diffPolygon) id--;
    return PolyhedronPolygons[id];
}

void ElementsRelations::addNeighborPolygonInPolygon(unsigned int id_polygon, unsigned int id_neighbor){
    if(diffPolygon) id_polygon--;
    PolygonsPolygons[id_polygon].getVectorIds().push_back(id_neighbor);
}

void ElementsRelations::addVertexInPolygon(unsigned int id_polygon, unsigned int id_vertex){
    if(diffPolygon) id_polygon--;
    VertexPolygons[id_polygon].getVectorIds().push_back(id_vertex);
}

void ElementsRelations::addPolygonInVertex(unsigned int id_vertex, unsigned int id_polygon){
    if(diffVertex) id_vertex--;
    PolygonsVertex[id_vertex].getVectorIds().push_back(id_polygon);
}

void ElementsRelations::addPolygonInPolyhedron(unsigned int id_polyhedron, unsigned int id_polygon){
    if(diffPolyhedron) id_polyhedron--;
    PolygonsPolyhedrons[id_polyhedron].getVectorIds().push_back(id_polygon);
}

void ElementsRelations::addPolyhedronInPolygon(unsigned int id_polyhedron, unsigned id_polygon){
    if(diffPolygon) id_polygon--;
    if(PolyhedronPolygons[id_polygon].count == 0) {
        PolyhedronPolygons[id_polygon].first_value = id_polyhedron;
        PolyhedronPolygons[id_polygon].count++;
    }
    else {
        PolyhedronPolygons[id_polygon].second_value = id_polyhedron;
        PolyhedronPolygons[id_polygon].count++;
    }
}

void ElementsRelations::addPolygonPolyhedronRelations(unsigned int id_polyhedron, unsigned int id_polygon){
    if(PolyhedronPolygons[id_polygon].count == 2) return;
    if(PolyhedronPolygons[id_polygon].count == 0) {
        PolyhedronPolygons[id_polygon].first_value = id_polyhedron;
        PolygonsPolyhedrons[id_polyhedron].getVectorIds().push_back(id_polygon);
        PolyhedronPolygons[id_polygon].count = 1;
    }
    else if(!PolyhedronPolygons[id_polygon].first_value == id_polyhedron){
        PolyhedronPolygons[id_polygon].second_value = id_polyhedron;
        PolygonsPolyhedrons[id_polyhedron].getVectorIds().push_back(id_polygon);
        PolyhedronPolygons[id_polygon].count = 2;
    }
}

void ElementsRelations::addIdAndPositionInContainerVertex(unsigned int vid, unsigned int vpos){
    if(vid != vpos) diffVertex = true;
    vertexIndexInPosition.insert({vid, vpos});
}

unsigned int ElementsRelations::getPositionInContainerById(unsigned int vid){
    if(!diffVertex){
        return vid;
    }
    else{
        return vid-1;
    }
}

void ElementsRelations::checkVertices(){
    if(!this->diffVertex){
        vertexIndexInPosition.clear();
    }
}

void ElementsRelations::setDiffVertex(bool value){
    diffVertex = value;
}

void ElementsRelations::setDiffPolygon(bool value){
    diffPolygon = value;
}

void ElementsRelations::setDiffPolyhedron(bool value){
    diffPolyhedron = value;
}

bool ElementsRelations::getDiffVertex(){
    return diffVertex;
}

bool ElementsRelations::getDiffPolygon(){
    return diffPolygon;
}

bool ElementsRelations::getDiffPolyhedron(){
    return diffPolyhedron;
}
