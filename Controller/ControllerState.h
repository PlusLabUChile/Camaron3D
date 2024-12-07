#pragma once
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include "Controller/ControllerContext.h"
#include "UI/CustomGLViewer.h"

/**
 * @brief Base Controller state class that have all the methods that need to be overriding in each
 * child class.
 * 
 * This class has different methods for pressing/releasing keys or using a mouse because
 * it use Qt methods of user inputs that use QtEvents, like QKeyEvent, QMouseEvent and QWheelEvent.
*/
class ControllerState {

    protected:
        /**
        * Store the ControllerContext the class that belongs in the Controller
        * State flow
        */
        ControllerContext* context;

    public:
        ControllerState();
        virtual ~ControllerState();
        void setControllerContext(ControllerContext* context);

        /**
         * Return the CustomGLViewer class instance
         */
        CustomGLViewer* getViewer();

        /**
         * Init state of controllerState
         * 
         */
        virtual void init();

        /**
         * Init state of controllerState
         * 
         */
        virtual void end();

        /**
         * Handle a event where the user press a key 
         * @param QKeyEvent Qt event that has the information 
         */
        virtual void handleKeyPressEvent(QKeyEvent*);

        /**
         * Handle a event where the user release a key 
         * @param QKeyEvent Qt event that has the information 
         */
        virtual void handleKeyReleaseEvent(QKeyEvent*);

        /**
         * Handle a event where the user press a mouse's button
         * @param QKeyEvent Qt event that has the information 
         */
        virtual void handleMouseReleaseEvent(QMouseEvent*);

        /**
         * Handle a event where the user release a mouse's button
         * @param QKeyEvent Qt event that has the information 
         */
        virtual void handleMousePressEvent(QMouseEvent*);

        /**
         * Handle a event where the user use the mouse's wheel
         * @param QKeyEvent Qt event that has the information 
         */
        virtual void handleWheelEvent(QWheelEvent*);

        /**
         * Handle a event where the user move the mouse 
         * @param QKeyEvent Qt event that has the information 
         */
        virtual void handleMouseMoveEvent(QMouseEvent*);
};