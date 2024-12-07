#pragma once
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <glm/glm.hpp>
#include "Utils/keyboardstate.h"
#include "Rendering/RModel/rmodel.h"

// List of Controller State
class ControllerState;
class ModelControllerState;
class SphereInterController;
class PlaneControllerState;
class CustomGLViewer;

/**
* @brief Controls the state for Camaron Controller
*
* This class use the pattern State (https://refactoring.guru/es/design-patterns/state) which is a behavioral design pattern 
* that lets an object alter its behavior when its internal state changes.
*
* This class will have different Controller State where each one will modify values of Camaron depending of the actual context.
*
* At the moment there are 4 states:
*   - ModelController  
*       Main controller of the Camaron Model. This class modify values in Camera and Viewer.
*   - SphereInterController
*       Controller for Sphere Geometry Intersection. This can be activate with the key H and if the user is using the sphere geometry option
*       for geometry intersection rendering.
*   - PlaneController
*       Controller for Plane Geometry Intersection. This can be activate with the key H and if the user is using the planes geometry option
*       for geometry intersection rendering.
*
* Also, this class has a pointer to viewer. At the first moment each controller state had a pointer to that class but it was a redundant variable
* that can be access if the states has a reference of ControllerContext.
*
* Request inputs methods are called in CustomGLViewer and those will call the actual state methods for that event.
*/

class ControllerContext{
    private:
        // Actual controller state
        ControllerState* state;

    public:
        ControllerContext();
        ~ControllerContext();

        /**
         * Change the actual state with the given one
         * @param ControllerState State that will be the new actual state
         */
        void transitionTo(ControllerState*);

        /**
         * Recive a QKeyEvent and call the actual state function to handle
         * @param ControllerState State that will be the new actual state
         */
        void requestKeyPressEvent(QKeyEvent*);

        /**
         * Recive a QKeyEvent and call the actual state function to handle
         * @param ControllerState State that will be the new actual state
         */
        void requestKeyReleaseEvent(QKeyEvent*);

        /**
         * Recive a QWheelEvent and call the actual state function to handle
         * @param ControllerState State that will be the new actual state
         */
        void requestMouseWheelEvent(QWheelEvent*);

        /**
         * Recive a QMouseEvent and call the actual state function to handle
         * @param ControllerState State that will be the new actual state
         */
        void requestMousePressEvent(QMouseEvent*);

        /**
         * Recive a QMouseEvent and call the actual state function to handle
         * @param ControllerState State that will be the new actual state
         */
        void requestMouseMoveEvent(QMouseEvent*);

        /**
         * Recive a QMouseEvent and call the actual state function to handle
         * @param ControllerState State that will be the new actual state
         */
        void requestMouseReleaseEvent(QMouseEvent*);

        // Get-Set function of viewer
        /**
         * Getter and Setter functions of CustomGLViewer
         */
        void setViewer(CustomGLViewer*);
        CustomGLViewer* getViewer();

        /**
         * Set values of MaxValueMod, MinValueMod and MaxLengthModel
         * @param newMaxValue 
         * @param newMinValue 
         * @param newLengthValue 
         */
        void setStepRangeModel(float, float, float);

        /**
         * Return private ControllerContext parameter MaxValueMod
         */
        float getMaxValueMod();

        /**
         * Return private ControllerContext parameter MinValueMod
         */
        float getMinValueMod();

        /**
         * Return private ControllerContext parameter ActualValueMod.
         * That parameter is used to change values of model and ConvexGeometry Figures (Sphere and Planes) in the 3D Spaces.
         */
        float getActualValueMod();

        /**
         * Return private ControllerContext parameter MaxLenghtModel.
         */
        float getMaxLengthModel();

        /**
         * Reset parameters of all ControllerStates to theirs defaults values
         */
        void reset();


        // Parameters for keyboard inputs
        KeyboardState keyboardState;
        QPoint lastPos;
        CustomGLViewer* viewer;

        // Keyboard/Mouse Controller State
        ControllerState* modelController;
        ControllerState* sphereController;
        ControllerState* planeController;

    private:
        // Modifications values
        float maxValueMod, minValueMod, actualValueMod, maxLengthModel;
        
};