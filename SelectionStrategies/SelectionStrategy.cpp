#include "SelectionStrategies/SelectionStrategy.h"
#include "SelectionStrategies/Selection.h"
#include "Model/Element/Element.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Model/Model.h"
#include "Rendering/RModel/rmodel.h"
#include <Utils/crosstimer.h>
#include <iostream>


SelectionStrategy::SelectionStrategy(std::string newname):
	name(newname)
{
	applyOnSelection = false;
	weight = SELECTION_STRATEGY_BASE_WEIGHT;
}

SelectionStrategy::~SelectionStrategy()
{
	//dtor
}
bool SelectionStrategy::isFullFilled( vis::Vertex* ) { return false;}
bool SelectionStrategy::isFullFilled( vis::Polygon* ) { return false;}
bool SelectionStrategy::isFullFilled( vis::Polyhedron* ) { return false;}
bool SelectionStrategy::isFullFilled( vis::Edge* ) { return false;}
bool SelectionStrategy::isFullFilled( Selection* ) { return true;}
bool SelectionStrategy::selectElement( vis::Polyhedron *, Selection* ) {return false;}
bool SelectionStrategy::selectElement( vis::Polygon *, Selection* ) {return false;}
bool SelectionStrategy::selectElement( vis::Vertex *, Selection* ) {return false;}
bool SelectionStrategy::selectElement( vis::Edge *, Selection* ) {return false;}
void SelectionStrategy::selectElement( Selection* sel) {
	std::unordered_map<int, vis::Element*>& hash = sel->getSelectedElements();
	std::vector<vis::Element*> erasedKeys;
	for ( std::unordered_map<int, vis::Element*>::const_iterator It = hash.begin(); It != hash.end(); ++It )
		if(!( *( *It ).second ).applySelectionStrategyDD(this,sel))
			erasedKeys.push_back((*It).second);
	for ( vis::Element* element : erasedKeys )
		sel->removeSelectedElement(sel->getRModel()->getOriginalModel(),element);
}
bool SelectionStrategy::setup(){return true;}
bool SelectionStrategy::setupPreApplying(Selection*, RModel*){return true;}

const char* SelectionStrategy::getName( ) const{
	return this->name.c_str();
}
bool  SelectionStrategy::hasConfigWidget(){
	return false;
}
QWidget*  SelectionStrategy::getConfigWidget(){
	return (QWidget*)0;
}
bool SelectionStrategy::isAppliedOnSelection(){
	return applyOnSelection;
}

/***
*  @brief Tells the interface if we need an "Apply button" to start the strategy
*
*  Selection strategies don't define this button on their own, instead the Visualizdor
*  class defines an apply button for the selection submenu, which is toggled depending
*  on the return value of this method. If a selection doesn't need one, it will
*  override this method and set it to false.
*
*  @todo See if you can decouple the apply button from the visualizador class
*  to somewhere else, as a way to reduce its complexity.
*/
bool SelectionStrategy::applyButtonEnabled(){
	return true;
}

bool SelectionStrategy::selectElementsFrom(Model* model, RModel* rmodel,
										   Selection* selection,
										   bool setupPreApplied){

	if(!setupPreApplied){
		if(!setupPreApplying(selection,rmodel)){
			return false;
			}
		}

	if( isFullFilled((vis::Polyhedron*)0)){
		std::vector<vis::Polyhedron>& polyhedrons = model->getPolyhedrons();
		for(vis::Polyhedron& polyhedron : polyhedrons)
			selectElement(&polyhedron,selection);
		return true;
		}

	if( isFullFilled((vis::Polygon*)0)){
		std::vector<vis::Polygon>& polygons = model->getPolygons();
		for( vis::Polygon& polygon : polygons )
			selectElement(&polygon,selection);
		return true;
		}

	if( isFullFilled((vis::Vertex*)0)){
		return false;
		//Not ready
		}

	return false;
	}

void SelectionStrategy::QApplicationInitiated(){

}
float SelectionStrategy::getWeight(){
	return weight;
}

