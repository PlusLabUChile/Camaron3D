#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

class OpenGLContext{
	public:
		int width, height;
	
		OpenGLContext(int mon_x, int mon_y);
		~OpenGLContext();
		void setupContext();
		void setupOpenGL();
		GLFWwindow* window;
	private:
		//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		};