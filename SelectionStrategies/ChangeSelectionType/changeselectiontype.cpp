#include "changeselectiontype.h"
#include "changeselectiontypeconfig.h"
#include "SelectionStrategies/Selection.h"
#include "Common/Constants.h"
#include "Model/Model.h"
#include "Model/Element/Polygon.h"
#include "Model/Element/Polyhedron.h"
#include "Rendering/RModel/rmodel.h"
ChangeSelectionType::ChangeSelectionType():
	SelectionStrategy("Change Type")
{
	config = (ChangeSelectionTypeConfig*)0;
	weight = SELECTION_STRATEGY_BASE_WEIGHT+10.0f;
	applyOnSelection = true;
}
ChangeSelectionType::~ChangeSelectionType()
{
	if(config)
		delete config;
}
void ChangeSelectionType::selectElement( Selection* sel ) {
	Model* model = sel->getRModel()->getOriginalModel();
	std::unordered_map<int,vis::Element*> newSelection;
	if(config->hint){//all must be selected
		switch(sel->getSelectionType()){
			case vis::CONSTANTS::POLYGON:{
				for ( std::unordered_map<int, vis::Element*>::const_iterator It = sel->getSelectedElements().begin();
					  It != sel->getSelectedElements().end(); ++It ){
					vis::Polygon* selected = (vis::Polygon*)( *It ).second;
					// vis::Polyhedron** neighbors = selected->getNeighborPolyhedron();
					relationPair neighbors = model->getElementsRelations()->getPolyhedronPolygonsById(selected->getId());
					if(neighbors.count < 0){
						std::vector<unsigned int>& polygonsOfPolyhedron = model->getElementsRelations()->getPolygonsPolyhedronsById(neighbors.first_value);
						bool addPolyhedron = true;
						for(unsigned int polID : polygonsOfPolyhedron){
							if(!model->getPolygons()[polID].isSelected()){
								addPolyhedron = false;
								break;
							}
						}
						if (addPolyhedron)
							newSelection[neighbors.first_value] = &model->getPolyhedrons()[neighbors.first_value];

					}
					if(neighbors.count == 2){
						std::vector<unsigned int>& polygonsOfPolyhedron = model->getElementsRelations()->getPolygonsPolyhedronsById(neighbors.second_value);
						bool addPolyhedron = true;
						for(unsigned int polID : polygonsOfPolyhedron){
							if(!model->getPolygons()[polID].isSelected()){
								addPolyhedron = false;
								break;
							}
						}
						if (addPolyhedron)
							newSelection[neighbors.second_value] = &model->getPolyhedrons()[neighbors.second_value];
					}
				}
				break;
			}
			case vis::CONSTANTS::VERTEX:{
				//Vertex* selected = (Vertex*)( *It ).second;
				break;
			}
			case vis::CONSTANTS::POLYHEDRON:{
				for ( std::unordered_map<int, vis::Element*>::const_iterator It = sel->getSelectedElements().begin();
					  It != sel->getSelectedElements().end(); ++It )
				{
					vis::Polyhedron* selected = (vis::Polyhedron*)( *It ).second;
					// std::vector<vis::Polygon*>& polyhedronPolygons = selected->getPolyhedronPolygons();
					std::vector<unsigned int>& polygonsOfPolyhedron = model->getElementsRelations()->getPolygonsPolyhedronsById(selected->getId());
					for(unsigned int polID : polygonsOfPolyhedron){
						relationPair polyhedronPolygon = model->getElementsRelations()->getPolyhedronPolygonsById(polID);
						unsigned int otherPolyhedron;
						(unsigned int) polyhedronPolygon.first_value == selected->getId() ? 
							otherPolyhedron = polyhedronPolygon.second_value : 
							otherPolyhedron = polyhedronPolygon.first_value;
						bool addPolygon = true;
						if(polyhedronPolygon.count != 0)
							addPolygon = model->getPolyhedrons()[otherPolyhedron].isSelected();
						if(addPolygon)
							newSelection[polID] = &model->getPolygons()[polID];
					}
				}
				break;
			}
		}
	}
	else{
		switch(sel->getSelectionType()){
			case vis::CONSTANTS::POLYGON:{
				for ( std::unordered_map<int, vis::Element*>::const_iterator It = sel->getSelectedElements().begin();
					  It != sel->getSelectedElements().end(); ++It ){
					vis::Polygon* selected = (vis::Polygon*)( *It ).second;
					// vis::Polyhedron** neighbors = selected->getNeighborPolyhedron();
					relationPair polyhedronPol = model->getElementsRelations()->getPolyhedronPolygonsById(selected->getId());
					if(polyhedronPol.count > 0){
						newSelection[polyhedronPol.first_value] = &model->getPolyhedrons()[polyhedronPol.first_value];
						if(polyhedronPol.count == 2)
							newSelection[polyhedronPol.second_value] = &model->getPolyhedrons()[polyhedronPol.second_value];
					}
				}
				break;
			}
			case vis::CONSTANTS::VERTEX:{
				//Vertex* selected = (Vertex*)( *It ).second;
				break;
			}
			case vis::CONSTANTS::POLYHEDRON:{
				for ( std::unordered_map<int, vis::Element*>::const_iterator It = sel->getSelectedElements().begin();
					  It != sel->getSelectedElements().end(); ++It )
				{
					vis::Polyhedron* selected = (vis::Polyhedron*)( *It ).second;
					// std::vector<vis::Polygon*>& polyhedronPolygons = selected->getPolyhedronPolygons();
					std::vector<unsigned int> polygonsIDS = model->getElementsRelations()->getPolygonsPolyhedronsById(selected->getId());
					for(unsigned int polID : polygonsIDS)
						newSelection[polID] = &model->getPolygons()[polID];
				}
				break;
			}
		}
	}
	sel->clearSelectedElements();
	sel->setSelectionType(config->elementsType);
	for ( std::unordered_map<int, vis::Element*>::const_iterator It = newSelection.begin();
		  It != newSelection.end(); ++It ){
		sel->addSelectedElement(( *It ).second);
	}
}
bool ChangeSelectionType::isFullFilled( Selection* sel ) {
	return sel->getSelectionType()!=config->elementsType &&
									sel->getSelectionType() != Selection::NOT_INITIALIZED;
}
bool ChangeSelectionType::setup(){
	config->readConfig();
	return true;
}
bool ChangeSelectionType::setupPreApplying(Selection* sel, RModel * rmodel){
	if(sel->getSelectionType()==config->elementsType)
		return false;//same type, no change
	if(config->elementsType == vis::CONSTANTS::POLYGON){
		if(rmodel->getOriginalModel()->getPolygonsCount() == 0)
			return false;//model has no polygons
	}
	if(config->elementsType == vis::CONSTANTS::POLYHEDRON){
		if(rmodel->getOriginalModel()->getPolyhedronsCount() == 0)
			return false;//model has no polyhedrons
	}
	return true;
}
bool ChangeSelectionType::hasConfigWidget(){
	return true;
}

QWidget* ChangeSelectionType::getConfigWidget(){
	return this->config;
}
void ChangeSelectionType::QApplicationInitiated(){
	config = new ChangeSelectionTypeConfig();
}

#include "Factories/selectionstrategyregistry.h"
REGISTER_SELECTION_STRATEGY(ChangeSelectionType);
