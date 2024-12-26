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

    maxValueMod = 0.0f;
    minValueMod = 1.0f;
    actualValueMod = 0.5f;
    maxLengthModel = 100.0f;
}

ControllerContext::~ControllerContext(){};

void ControllerContext::transitionTo(ControllerState* s){
    this->state->end();
    this->state = s;
    this->state->init();
}

CustomGLViewer* ControllerContext::getViewer(){
    return this->viewer;
}

void ControllerContext::setViewer(CustomGLViewer* viewer){
    this->viewer = viewer;
}

void ControllerContext::reset(Model* model){
    transitionTo(this->modelController);
    float step_count = 1000.0f;
    std::vector<float> bounds = model->getBounds();
    if(bounds.size() > 0){
        float var_value = glm::length(
            glm::vec3(
                bounds[3],
                bounds[4],
                bounds[5]
            )
        );
        setStepRangeModel(1/step_count, var_value/step_count, var_value);
    }
}

void ControllerContext::setStepRangeModel(float min, float max, float total)
{
    maxLengthModel = total; minValueMod = min; maxValueMod = max;
    actualValueMod = max * 0.9;
    getViewer()->applyRendererConfigChanges();
}

float ControllerContext::getMaxValueMod()
{
    return maxValueMod;
}

float ControllerContext::getMinValueMod()
{
    return minValueMod;
}

float ControllerContext::getActualValueMod()
{
    return actualValueMod;
}

float ControllerContext::getMaxLengthModel()
{
    return maxLengthModel;
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