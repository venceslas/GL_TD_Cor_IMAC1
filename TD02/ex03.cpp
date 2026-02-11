#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "tools/shaders.hpp"
#include <iostream>

using namespace glbasimac;
using namespace STP3D;

/* Window properties */
static const unsigned int WINDOW_WIDTH = 800;
static const unsigned int WINDOW_HEIGHT = 800;
static const char WINDOW_TITLE[] = "TD02 Ex03";
static float aspectRatio = 1.0f;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* Virtual windows space */
static const float GL_VIEW_SIZE = 4.;

/* OpenGL Engine */
GLBI_Engine myEngine;
GLBI_Set_Of_Points thePoints;
GLBI_Set_Of_Points repere;

/* Error handling function */
void onError(int error, const char* description) {
	std::cout << "GLFW Error ("<<error<<") : " << description << std::endl;
}

void onWindowResized(GLFWwindow* /*window*/, int width, int height)
{
	aspectRatio = width / (float) height;

	glViewport(0, 0, width, height);
	if( aspectRatio > 1)
	{
		myEngine.set2DProjection(-GL_VIEW_SIZE / 2. * aspectRatio, GL_VIEW_SIZE / 2. * aspectRatio,
								 -GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.);
	}
	else
	{
		myEngine.set2DProjection(-GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.,
								 -GL_VIEW_SIZE / 2. / aspectRatio, GL_VIEW_SIZE / 2. / aspectRatio);
	}
}

void onKey(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		thePoints.changeNature(GL_LINE_STRIP);
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		thePoints.changeNature(GL_POINTS);
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		thePoints.changeNature(GL_LINE_LOOP);
	}
	if (key == GLFW_KEY_T && action == GLFW_PRESS) {
		thePoints.changeNature(GL_TRIANGLES);
	}
}

void onMouseButton(GLFWwindow* window, int button, int action, int /*mods*/)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout<<"Pressed in "<<xpos<<" "<<ypos<<std::endl;

		int s_w,s_h;
		glfwGetWindowSize(window,&s_w,&s_h);
		std::vector<float> coord;
		std::vector<float> color;
		/* 
		CONSIGNE TD : les deux équations ci-dessous à éventuellement donner en TD si les étudiants galèrent trop
		(-1 + 2. * pos_button.x / (float) width_window) * (GL_VIEW_SIZE / 2)
		( 1 - 2. * pos_button.y / (float) height_window) * (GL_VIEW_SIZE / 2)
		*/
		if (aspectRatio>1.0) {
			coord.push_back( (-1 + 2. * xpos / (float) s_w) * aspectRatio * (GL_VIEW_SIZE / 2) );
			coord.push_back( (1 - 2. * ypos / (float) s_h) * (GL_VIEW_SIZE / 2) );
		}
		else {
			coord.push_back( (-1 + 2. * xpos / (float) s_w) * (GL_VIEW_SIZE / 2) );
			coord.push_back( (1 - 2. * ypos / (float) s_h) * (GL_VIEW_SIZE / 2) / aspectRatio );
		}
		color.push_back(0.5+0.5*xpos/(float)s_w);
		color.push_back(0.5+0.5*ypos/(float)s_h);
		color.push_back(0.0);
		thePoints.addAPoint(coord.data(),color.data());
	}
}


void initScene() {
	std::vector<float> points {0.0,0.0};
	std::vector<float> col{1.0,1.0,1.0};
	thePoints.initSet(points,col);

	// Exercice Optionnel 03
	std::vector<float> r_points {0.0,0.0,1.0,0.0,0.0,0.0,0.0,1.0};
	std::vector<float> r_col{1.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0,0.0,0.0,1.0,0.0};
	repere.initSet(r_points,r_col);
	repere.changeNature(GL_LINES);
}

void renderScene() {
	glPointSize(4.0);
	thePoints.drawSet();

	repere.drawSet();
}

int main(int /*argc*/, char** /*argv*/)
{
	/* GLFW initialisation */
	GLFWwindow* window;
	if (!glfwInit()) return -1;

	/* Callback to a function if an error is rised by GLFW */
	glfwSetErrorCallback(onError);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		// If no context created : exit !
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	std::cout<<"Loading GL extension"<<std::endl;
	// Intialize glad (loads the OpenGL functions)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return -1;
	}

	glfwSetWindowSizeCallback(window,onWindowResized);
	glfwSetKeyCallback(window, onKey);
	glfwSetMouseButtonCallback(window, onMouseButton);


	std::cout<<"Engine init"<<std::endl;
	myEngine.initGL();
	onWindowResized(window,WINDOW_WIDTH,WINDOW_HEIGHT);
	CHECK_GL;

	initScene();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Render here */
		glClearColor(0.2f,0.0,0.0,0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		renderScene();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		/* Elapsed time computation from loop begining */
		double elapsedTime = glfwGetTime() - startTime;
		/* If to few time is spend vs our wanted FPS, we wait */
		if(elapsedTime < FRAMERATE_IN_SECONDS)
		{
			glfwWaitEventsTimeout(FRAMERATE_IN_SECONDS-elapsedTime);
		}
	}

	glfwTerminate();
	return 0;
}
