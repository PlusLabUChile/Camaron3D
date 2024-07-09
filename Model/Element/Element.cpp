#include "Model/Element/Element.h"
vis::Element::Element( unsigned int newId ){
	this->id = newId;
	this->selected = false;
	}

vis::Element::~Element() {}

unsigned int vis::Element::getId() {
	return this->id;
}

bool vis::Element::isSelected() {
	return this->selected;
}
void vis::Element::setSelected( bool s ) {
	this->selected = s;
}
std::vector<int>& vis::Element::getRmodelPositions(){
	return rmodelPositions;
}

void vis::Element::addRmodelPositions(int pos){
	rmodelPositions.push_back(pos);
}
