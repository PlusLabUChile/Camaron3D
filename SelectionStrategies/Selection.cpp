#include "SelectionStrategies/Selection.h"
#include "Model/Model.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Common/Constants.h"
#include "Rendering/RModel/rmodel.h"
#include "Rendering/RModel/RVertexFlagAttribute.h"
Selection::Selection()
{
	rmodel = (RModel*)0;
	selectionType = NOT_INITIALIZED;
}

Selection::~Selection()
{
}
std::unordered_map<int, vis::Element*>& Selection::getSelectedElements() {
	return this->selectedElements;
}

void Selection::clearSelectedElements() {
	for ( std::unordered_map<int, vis::Element*>::const_iterator It = this->selectedElements.begin(); It != this->selectedElements.end(); ++It )
	{
		// Print both the key and the value of that key
		// First = Key (Int), Second = Value of that key (String)
		( *( *It ).second ).setSelected( false );
		std::vector<int>& rmodelPositions = ( *( *It ).second ).getRmodelPositions();
		std::vector<RVertexFlagAttribute>& flagAttributes = rmodel->vertexFlagsAttribute;
		for( int position : rmodelPositions ){
			RVertexFlagAttribute& flags = flagAttributes[position];
			flags.disableFlag(RVertexFlagAttribute::SELECTED_FLAG);
		}
	}
	this->selectedElements.clear();
}
bool Selection::addSelectedElement( vis::Element* selectedElement ) {
	//if(selectedElement->isSelected())
	//	return true;//already selected
	selectedElement->setSelected( true );
	this->selectedElements[selectedElement->getId()] = selectedElement;
	std::vector<int>& rmodelPositions = selectedElement->getRmodelPositions();
	std::vector<RVertexFlagAttribute>& flagAttributes = rmodel->vertexFlagsAttribute;
	for( int position : rmodelPositions )
		flagAttributes[position].enableFlag(RVertexFlagAttribute::SELECTED_FLAG);
	return true;
}
bool Selection::removeSelectedElement( Model* model, vis::Element* selectedElement){
	if(selectionType == vis::CONSTANTS::POLYHEDRON)
		return removeSelectedElement(model, (vis::Polyhedron*)selectedElement);
	selectedElement->setSelected( false );
	std::vector<int>& rmodelPositions = selectedElement->getRmodelPositions();
	std::vector<RVertexFlagAttribute>& flagAttributes = rmodel->vertexFlagsAttribute;
	for( int position : rmodelPositions ){
		RVertexFlagAttribute& flags = flagAttributes[position];
		flags.disableFlag(RVertexFlagAttribute::SELECTED_FLAG);
	}
	if(selectedElements.count(selectedElement->getId())>0)
		this->selectedElements.erase(selectedElement->getId());
	return true;
}
bool Selection::removeSelectedElement( Model* model, vis::Polyhedron* selectedPolyhedron){
	selectedPolyhedron->setSelected( false );
	if(selectedElements.count(selectedPolyhedron->getId())>0)
		this->selectedElements.erase(selectedPolyhedron->getId());
	std::vector<RVertexFlagAttribute>& flagAttributes = rmodel->vertexFlagsAttribute;
	for( unsigned int polygonID : model->getElementsRelations()->getPolygonsPolyhedronsById(selectedPolyhedron->getId()) ){
		// vis::Polyhedron* neighbor = polygon->getNeighborPolyhedron(selectedPolyhedron);
		unsigned int neighbor = model->getElementsRelations()->getPolyhedronPolygonsById(polygonID).first_value;
		if(neighbor == ((unsigned int)selectedPolyhedron->getId())) 
			neighbor = model->getElementsRelations()->getPolyhedronPolygonsById(polygonID).second_value;
		if(model->getElementsRelations()->getPolyhedronPolygonsById(polygonID).count != 0 && model->getPolyhedrons()[neighbor].isSelected())
			continue;//must ignore this polygon
		std::vector<int>& rmodelPositions = model->getPolygons()[polygonID].getRmodelPositions();
		for( int position : rmodelPositions ){
			RVertexFlagAttribute& flags = flagAttributes[position];
			flags.disableFlag(RVertexFlagAttribute::SELECTED_FLAG);
		}
	}
	return true;
}

void Selection::setSelectionType( int b){
	this->selectionType = b;
}
int Selection::getSelectionType(){
	return this->selectionType;
}
void Selection::setRModel(RModel* rmodel){
	this->rmodel = rmodel;
}

RModel* Selection::getRModel(){
	return this->rmodel;
}
void Selection::reset(){
	selectionType = NOT_INITIALIZED;
	selectedElements.clear();
}
