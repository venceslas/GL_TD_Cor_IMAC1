#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "glbasimac/glbi_engine.hpp"
#include "glbasimac/glbi_set_of_points.hpp"
#include "glbasimac/glbi_convex_2D_shape.hpp"
#include "tools/shaders.hpp"
#include <iostream>

using namespace glbasimac;
using namespace STP3D;

/* Window properties */
static const unsigned int WINDOW_WIDTH = 800;
static const unsigned int WINDOW_HEIGHT = 800;
static const char WINDOW_TITLE[] = "TD02 Ex04";
static float aspectRatio = 1.0f;
static int typeObject = 0;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* Virtual windows space */
static const float GL_VIEW_SIZE = 4.;

/* OpenGL Engine */
GLBI_Engine myEngine;
GLBI_Convex_2D_Shape carre;
GLBI_Convex_2D_Shape cercle;
GLBI_Convex_2D_Shape triangle;

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
	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		typeObject = (typeObject+1)%3;
	}
}

void onMouseButton(GLFWwindow* window, int button, int action, int /*mods*/)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout<<"Pressed in "<<xpos<<" "<<ypos<<std::endl;

	}
}


void initScene() {
	// Le carré
	std::vector<float> pt_cr {-0.5,-0.5,0.5,-0.5,0.5,0.5,-0.5,0.5};
	carre.initShape(pt_cr);
	carre.changeNature(GL_TRIANGLE_FAN);
	// Le triangla
	std::vector<float> pt_tr {-0.5,-0.5,0.5,-0.5,0.0,0.5};
	triangle.initShape(pt_tr);
	triangle.changeNature(GL_TRIANGLE_FAN);
	// Le cercle
	std::vector<float> pt_cercle;
	float angle = 0.0;
	for(int i=0;i<50;i++) {
		angle = i*2.*M_PI/(float)50;
		pt_cercle.push_back(0.5*cos(angle));
		pt_cercle.push_back(0.5*sin(angle));
	}
	cercle.initShape(pt_cercle);
	cercle.changeNature(GL_TRIANGLE_FAN);
}

void renderScene() {
	glPointSize(4.0);

	myEngine.updateMvMatrix();

	if (typeObject == 0) {
		myEngine.setFlatColor(1.0,1.0,0.0);
		carre.drawShape();
	}
	if (typeObject == 1) {
	 	myEngine.setFlatColor(0.0,1.0,1.0);
	 	triangle.drawShape();
	}
	if (typeObject == 2) {
		myEngine.setFlatColor(1.0,0.0,0.0);
	 	cercle.drawShape();
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

		myEngine.updateMvMatrix();

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
