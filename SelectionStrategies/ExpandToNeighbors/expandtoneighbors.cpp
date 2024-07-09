#include "expandtoneighbors.h"
#include "SelectionStrategies/Selection.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Utils/PolygonUtils.h"
#include "Common/Constants.h"
#include "Rendering/RModel/rmodel.h"
#include "SelectionStrategies/ExpandToNeighbors/expandtoneighborsconfig.h"
ExpandToNeighbors::ExpandToNeighbors():SelectionStrategy("Expand Selection"){
	applyOnSelection = true;
	weight = SELECTION_STRATEGY_BASE_WEIGHT;
	config = (ExpandToNeighborsConfig*)0;
}
ExpandToNeighbors::~ExpandToNeighbors(){
	if(config)
		delete config;
}


void ExpandToNeighbors::selectElement( Selection * sel ) {
	if(config->onlySurface){
		expandSelectionSurface(sel);
		return;
	}
	if(config->onlySurfaceWithAngle){
		expandSelectionSurfaceWithAngle(sel,config->dihedralAngle);
		return;
	}
	this->expandSelectionAll(sel);
}
void ExpandToNeighbors::expandSelectionSurface(Selection* sel){
	Model* model = sel->getRModel()->getOriginalModel();
	ElementsRelations* relations = model->getElementsRelations();
	std::unordered_map<int, vis::Element*>& selectedElements = sel->getSelectedElements();
	std::unordered_map<int, vis::Element*> newSelectedElements;
	int selectionType = sel->getSelectionType();
	switch(selectionType){
		case vis::CONSTANTS::POLYGON:{
			for ( std::unordered_map<int, vis::Element*>::const_iterator It = selectedElements.begin(); It != selectedElements.end(); ++It )
			{
				vis::Polygon* selected = (vis::Polygon*)( *It ).second;
				std::vector<unsigned int>& neighborPolygonsIDS = relations->getPolygonsPolygonsById(selected->getId());
				for( unsigned int polygonID : neighborPolygonsIDS ){
					vis::Polygon* polygon = &model->getPolygons()[polygonID];
					if(PolygonUtils::isPolygonAtSurface(model, polygon) &&
							!polygon->isSelected())
						newSelectedElements[polygon->getId()] = polygon;
				}
			}
			break;
		}
		case vis::CONSTANTS::VERTEX:{
			//Vertex* selected = (Vertex*)( *It ).second;
			break;
		}
		case vis::CONSTANTS::POLYHEDRON:{
			for ( std::unordered_map<int, vis::Element*>::const_iterator It = selectedElements.begin(); It != selectedElements.end(); ++It )
			{
				vis::Polyhedron* selected = (vis::Polyhedron*)( *It ).second;
				std::vector<unsigned int>& polygonsIDS = relations->getPolygonsPolyhedronsById(selected->getId());
				for( unsigned int polygonID : polygonsIDS ){
					vis::Polygon* polygon = &model->getPolygons()[polygonID];
					if(!PolygonUtils::isPolygonAtSurface(model, polygon))
						continue;
					std::vector<unsigned int>& neighborPolygons = relations->getPolygonsPolygonsById(polygon->getId());
					for(unsigned int neighborID : neighborPolygons ){
						vis::Polygon* neighbor = &model->getPolygons()[neighborID];
						if(!PolygonUtils::isPolygonAtSurface(model, neighbor))
							continue;
						relationPair polyhedronsPolygon = relations->getPolyhedronPolygonsById(neighborID);
						unsigned int poplyhedronID = polyhedronsPolygon.first_value;
						if(poplyhedronID == ((unsigned int)selected->getId())) poplyhedronID = polyhedronsPolygon.second_value;
						vis::Polyhedron* polAsoc = &model->getPolyhedrons()[poplyhedronID];
						if(!polAsoc->isSelected())
							newSelectedElements[polAsoc->getId()] = polAsoc;
					}

				}
			}
			break;
		}
	}
	for ( std::unordered_map<int, vis::Element*>::const_iterator It = newSelectedElements.begin(); It != newSelectedElements.end(); ++It )
	{
		sel->addSelectedElement(( *It ).second);
	}
}

void ExpandToNeighbors::expandSelectionSurfaceWithAngle(Selection* sel, float angle){
	Model* model = sel->getRModel()->getOriginalModel();
	ElementsRelations* relations = model->getElementsRelations();
	std::unordered_map<int, vis::Element*>& selectedElements = sel->getSelectedElements();
	std::unordered_map<int, vis::Element*> newSelectedElements;
	int selectionType = sel->getSelectionType();
	switch(selectionType){
		case vis::CONSTANTS::POLYGON:{
			for ( std::unordered_map<int, vis::Element*>::const_iterator It = selectedElements.begin(); It != selectedElements.end(); ++It )
			{
				vis::Polygon* selected = (vis::Polygon*)( *It ).second;
				std::vector<unsigned int>& neighborPolygonsIDS = relations->getPolygonsPolygonsById(selected->getId());
				for( unsigned int polygonID : neighborPolygonsIDS ){
					vis::Polygon* polygon = &model->getPolygons()[polygonID];
					if(PolygonUtils::isPolygonAtSurface(model, polygon) &&
							!polygon->isSelected() &&
							PolygonUtils::getDihedralAngle(selected,polygon) <= angle)
						newSelectedElements[polygon->getId()] = polygon;
				}
			}
			break;
		}
		case vis::CONSTANTS::VERTEX:{
			//Vertex* selected = (Vertex*)( *It ).second;
			break;
		}
		case vis::CONSTANTS::POLYHEDRON:{
			for ( std::unordered_map<int, vis::Element*>::const_iterator It = selectedElements.begin(); It != selectedElements.end(); ++It )
			{
				vis::Polyhedron* selected = (vis::Polyhedron*)( *It ).second;
				std::vector<unsigned int>& polygonsIDS = relations->getPolygonsPolyhedronsById(selected->getId());
				for( unsigned int polygonID : polygonsIDS ){
					vis::Polygon* polygon = &model->getPolygons()[polygonID];
					if(!PolygonUtils::isPolygonAtSurface(model, polygon))  // if(!polygon->isAtSurface())
						continue;
					std::vector<unsigned int>& neighborPolygons = relations->getPolygonsPolygonsById(polygon->getId());
					for(unsigned int neighborID : neighborPolygons ){
						vis::Polygon* neighbor = &model->getPolygons()[neighborID];
						if(!PolygonUtils::isPolygonAtSurface(model, neighbor)) // if(!neighbor->isAtSurface())
							continue;
						relationPair polyhedronsPolygon = relations->getPolyhedronPolygonsById(neighborID);
						unsigned int poplyhedronID = polyhedronsPolygon.first_value;
						if(poplyhedronID == ((unsigned int)selected->getId())) poplyhedronID = polyhedronsPolygon.second_value;
						vis::Polyhedron* polAsoc = &model->getPolyhedrons()[poplyhedronID];						
						if(!polAsoc->isSelected() &&
								PolygonUtils::getDihedralAngle(polygon,neighbor) >= angle)
							newSelectedElements[polAsoc->getId()] = polAsoc;
					}

				}
			}
			break;
		}
	}
	for ( std::unordered_map<int, vis::Element*>::const_iterator It = newSelectedElements.begin(); It != newSelectedElements.end(); ++It )
	{
		sel->addSelectedElement(( *It ).second);
	}
}

void ExpandToNeighbors::expandSelectionAll(Selection* sel){
	Model* model = sel->getRModel()->getOriginalModel();
	ElementsRelations* relations = model->getElementsRelations();
	std::unordered_map<int, vis::Element*>& selectedElements = sel->getSelectedElements();
	std::unordered_map<int, vis::Element*> newSelectedElements;
	int selectionType = sel->getSelectionType();
	switch(selectionType){
		case vis::CONSTANTS::POLYGON:{
			for ( std::unordered_map<int, vis::Element*>::const_iterator It = selectedElements.begin(); It != selectedElements.end(); ++It )
			{
				vis::Polygon* selected = (vis::Polygon*)( *It ).second;
				std::vector<unsigned int>& neighborPolygonsIDS = relations->getPolygonsPolygonsById(selected->getId());
				for( unsigned int polygonID : neighborPolygonsIDS ){
					vis::Polygon* polygon = &model->getPolygons()[polygonID];
					if(!polygon->isSelected())
						newSelectedElements[polygon->getId()] = polygon;
				}
			}
			break;
		}
		case vis::CONSTANTS::VERTEX:{
			//Vertex* selected = (Vertex*)( *It ).second;
			break;
		}
		case vis::CONSTANTS::POLYHEDRON:{
			for ( std::unordered_map<int, vis::Element*>::const_iterator It = selectedElements.begin(); It != selectedElements.end(); ++It )
			{
				vis::Polyhedron* selected = (vis::Polyhedron*)( *It ).second;
				std::vector<unsigned int>& polygonsIDS = relations->getPolygonsPolyhedronsById(selected->getId());

				for( unsigned int polygonID : polygonsIDS ){
					relationPair polyhedronsPolygon = relations->getPolyhedronPolygonsById(polygonID);
					unsigned int poplyhedronID = polyhedronsPolygon.first_value;
					if(poplyhedronID == ((unsigned int)selected->getId())) poplyhedronID = polyhedronsPolygon.second_value;
					vis::Polyhedron* polAsoc = &model->getPolyhedrons()[poplyhedronID];
					if(polAsoc && !polAsoc->isSelected())
						newSelectedElements[polAsoc->getId()] = polAsoc;
				}
			}
			break;
		}
	}
	for ( std::unordered_map<int, vis::Element*>::const_iterator It = newSelectedElements.begin(); It != newSelectedElements.end(); ++It )
	{
		sel->addSelectedElement(( *It ).second);
	}
}

bool ExpandToNeighbors::isFullFilled( Selection * ) {
	return true;
}
bool ExpandToNeighbors::setup(){
	config->readConfig();
	return true;
}
bool ExpandToNeighbors::setupPreApplying(Selection*, RModel *){
	return true;
}
bool ExpandToNeighbors::hasConfigWidget(){
	return true;
}

QWidget* ExpandToNeighbors::getConfigWidget(){
	return this->config;
}
void ExpandToNeighbors::QApplicationInitiated(){
	config = new ExpandToNeighborsConfig();
}
#include "Factories/selectionstrategyregistry.h"
REGISTER_SELECTION_STRATEGY(ExpandToNeighbors);
