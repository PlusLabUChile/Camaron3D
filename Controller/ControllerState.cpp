#include "Controller/ControllerState.h"

ControllerState::ControllerState(){};
ControllerState::~ControllerState(){};
void ControllerState::setControllerContext(ControllerContext* context){
    this->context = context;
}
CustomGLViewer* ControllerState::getViewer(){
    return this->context->getViewer();
}
void ControllerState::handleKeyPressEvent(QKeyEvent* event){}
void ControllerState::handleKeyReleaseEvent(QKeyEvent* event){}
void ControllerState::handleMouseReleaseEvent(QMouseEvent* event){}
void ControllerState::handleMousePressEvent(QMouseEvent* event){}
void ControllerState::handleWheelEvent(QWheelEvent* event){}
void ControllerState::handleMouseMoveEvent(QMouseEvent* event){}