#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"
#include <iostream>

using namespace glbasimac;

/* Flag for fullscreen */
#define FULL_SCREEN 0

/* Window size */
static const unsigned int WINDOW_WIDTH = 800;
static const unsigned int WINDOW_HEIGHT = 600;
static float aspectRatio {1.0f};

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* Espace virtuel */
static const float GL_VIEW_SIZE = 1.;
// L'univers 2D visible a une taille de 1.0 en x et en y 

/* OpenGL Engine */
GLBI_Engine myEngine;

bool flag_clic_bg_color {false};
float bg_color[3] = {0.0,0.0,0.0};

/* Error handling function */
void onError(int error, const char* description) {
	std::cout << "GLFW Error ("<<error<<") : " << description << std::endl;
}

void onWindowResized(GLFWwindow* /*window*/, int width, int height)
{
	aspectRatio = width / (float) height;
	glViewport(0, 0, width, height);
	if( aspectRatio > 1.0)
	{
		myEngine.set2DProjection(-GL_VIEW_SIZE * aspectRatio/ 2.,
			GL_VIEW_SIZE * aspectRatio / 2. ,
			-GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.);
	}
	else
	{
		myEngine.set2DProjection(-GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.,
			 -GL_VIEW_SIZE / (2. * aspectRatio), 
			GL_VIEW_SIZE / (2. * aspectRatio));
	}
}

void onKey(GLFWwindow* window, int key, int /* scancode */, int action, int mods)
{
	// VERSION 1 : ENGLISH CONVERSION
	//std::cout<<"Key "<<key<<" "<<scancode<<" "<<action <<"!"<<std::endl;
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		std::cout<<"Ciao !"<<std::endl;
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		flag_clic_bg_color = !flag_clic_bg_color;
		if (flag_clic_bg_color) {
			// Reinit bg color to 0/0/0
			bg_color[0] = bg_color[1] = bg_color[2] = 0.0;
		}
	}
	if (flag_clic_bg_color) {
		switch(key) {
			case GLFW_KEY_R :
				if (mods&GLFW_MOD_SHIFT) bg_color[0] = std::max(0.0,bg_color[0]-0.01);
				else bg_color[0] = std::min(1.0,bg_color[0]+0.01);
				std::cout<<"bg_color_R="<<bg_color[0]<<std::endl;
				break;
			case GLFW_KEY_G :
				if (mods&GLFW_MOD_SHIFT) bg_color[1] = std::max(0.0,bg_color[1]-0.01);
				else bg_color[1] = std::min(1.0,bg_color[1]+0.01);
				std::cout<<"bg_color_G="<<bg_color[1]<<std::endl;
				break;
			case GLFW_KEY_B :
				if (mods&GLFW_MOD_SHIFT) bg_color[2] = std::max(0.0,bg_color[2]-0.01);
				else bg_color[2] = std::min(1.0,bg_color[2]+0.01);
				std::cout<<"bg_color_B="<<bg_color[2]<<std::endl;
				break;
		}
	}
	// VERSION 2 : WITH CHAR MAPPING...
	/*
	const char* keyName = glfwGetKeyName(key, scancode);
	if (key != GLFW_KEY_UNKNOWN && keyName != NULL) {
		if (action==GLFW_PRESS) std::cout<<"Test touche "<<keyName[0]<<std::endl;
		if( 'q' == keyName[0] && 0 == keyName[1] && action == GLFW_PRESS )
		{
			// have key q
			std::cout<<"Ciao !"<<std::endl;
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}
	*/
}

void onMouseButton(GLFWwindow* window, int button, int action, int /*mods*/)
{
	if(!flag_clic_bg_color) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			std::cout<<"Pressed in "<<xpos<<" "<<ypos<<std::endl;
			glClearColor(((int)xpos%255)/255.0,((int)ypos%255)/255.,0.0,0.0);
		}
	}
}

void onMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	if (!flag_clic_bg_color) {
		int s_w,s_h;
		glfwGetWindowSize(window,&s_w,&s_h);

		glClearColor(xpos/s_w,0.0,ypos/s_h,0.0);
	}
}

int main(int /*argc*/, char** /*argv*/) 
{
	/* GLFW initialisation */
	GLFWwindow* window;
	if (!glfwInit()) return -1;

	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

	/* Create a windowed mode window and its OpenGL context */
	if (FULL_SCREEN) {
		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TD 01 EX 04", glfwGetPrimaryMonitor(), NULL);
	}
	else {
		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TD 01 EX 04", NULL, NULL);
	}
	if (!window)
	{
		// If no context created : exit !
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Intialize glad (loads the OpenGL functions)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return -1;
	}

	glfwSetWindowSizeCallback(window, onWindowResized);
	glfwSetCursorPosCallback(window, onMouseMove);
	glfwSetMouseButtonCallback(window, onMouseButton);
	glfwSetKeyCallback(window, onKey);

	onWindowResized(window,WINDOW_WIDTH,WINDOW_HEIGHT);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		if (flag_clic_bg_color) {
			glClearColor(bg_color[0],bg_color[1],bg_color[2],0.0);
		}

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */
		double elapsedTime = glfwGetTime() - startTime;
		/* If to few time is spend vs our wanted FPS, we wait */
		while(elapsedTime < FRAMERATE_IN_SECONDS) 
		{
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS-elapsedTime);
			elapsedTime = glfwGetTime() - startTime;
		}
	}

	glfwTerminate();
	return 0;
}
