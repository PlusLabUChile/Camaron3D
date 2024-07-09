#include "Controller/ControllerContext.h"
#include "Controller/ControllerState.h"
#include "Controller/MainController/ModelControllerState.h"
#include "Controller/GeometryInterController/SphereInterController.h"
#include "Controller/GeometryInterController/PlaneControllerState.h"

ControllerContext::ControllerContext(){
    modelController = new ModelControllerState();
    modelController->setControllerContext(this);
    this->state = (ControllerState*) modelController;
    sphereController = new SphereInterController();
    sphereController->setControllerContext(this);
    planeController = new PlaneControllerState();
    planeController->setControllerContext(this);
}

ControllerContext::~ControllerContext(){};

void ControllerContext::transitionTo(ControllerState* s){
    this->state = s;
}

CustomGLViewer* ControllerContext::getViewer(){
    return this->viewer;
}

void ControllerContext::setViewer(CustomGLViewer* viewer){
    this->viewer = viewer;
}

void ControllerContext::requestKeyPressEvent(QKeyEvent* event){
    this->state->handleKeyPressEvent(event);
}

void ControllerContext::requestKeyReleaseEvent(QKeyEvent* event){
    this->state->handleKeyReleaseEvent(event);
}

void ControllerContext::requestMouseWheelEvent(QWheelEvent* event){
    this->state->handleWheelEvent(event);
}

void ControllerContext::requestMousePressEvent(QMouseEvent* event){
    this->state->handleMousePressEvent(event);
}

void ControllerContext::requestMouseReleaseEvent(QMouseEvent* event){
    this->state->handleMouseReleaseEvent(event);
}

void ControllerContext::requestMouseMoveEvent(QMouseEvent* event){
    this->state->handleMouseMoveEvent(event);
}