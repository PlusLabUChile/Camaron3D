#include "openglcontext.h"
#include <iostream>
//void framebuffer_size_callback(GLFWwindow* window, int width, int height){
//	  glViewport(0, 0, width, height);
//	}

OpenGLContext::OpenGLContext(int mon_x, int mon_y){
	width = mon_x;
	height = mon_y;
	}

OpenGLContext::~OpenGLContext(){
	glfwTerminate();
	}

void OpenGLContext::setupContext(){
	/***
	 * Initializes the GLFW context, returns a pointer to the newly created
	 * GLFWwindow object
	 */
	if(!glfwInit()){
		throw std::runtime_error("Unable to start GLFW");
		}

	window = glfwCreateWindow(width, height, "Test Window", NULL, NULL);

	if (!window){
		glfwTerminate();
		throw std::runtime_error("Unable to create an GLFW window");
		}

	glfwMakeContextCurrent(window);
	//glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Couldn't initalize OpenGL");
		}
	}

void OpenGLContext::setupOpenGL(){
	/***
	 * Sets up initial OpenGL parameters
	 */
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	glOrtho(0.0, width, 0.0, height, -50.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0);
	}


