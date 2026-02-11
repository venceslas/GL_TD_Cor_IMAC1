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
static const unsigned int WINDOW_WIDTH = 1200;
static const unsigned int WINDOW_HEIGHT = 1200;
static const char WINDOW_TITLE[] = "TD03 Ex01";
static float aspectRatio = 1.0f;

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS = 1. / 30.;

/* Virtual windows space */
static const float GL_VIEW_SIZE = 120.;

/* OpenGL Engine */
GLBI_Engine myEngine;
GLBI_Convex_2D_Shape carre;
GLBI_Convex_2D_Shape cercle;
GLBI_Convex_2D_Shape trapeze;

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
	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
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
	// Le trapèze
	std::vector<float> pt_tr {0.0,20.0,60.0,10.0,60.0,-10.0,0.0,-20.0};
	trapeze.initShape(pt_tr);
	trapeze.changeNature(GL_TRIANGLE_FAN);
	// Le cercle
	std::vector<float> pt_cercle;
	float angle = 0.0;
	for(int i=0;i<50;i++) {
		angle = i*2.*M_PI/(float)50;
		pt_cercle.push_back(cos(angle));
		pt_cercle.push_back(sin(angle));
	}
	cercle.initShape(pt_cercle);
	cercle.changeNature(GL_TRIANGLE_FAN);

	myEngine.mvMatrixStack.loadIdentity();
}

void drawFirstArm() {
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addHomothety(20.);
		myEngine.updateMvMatrix();
		myEngine.setFlatColor(0.25,0.25,0.25);
		cercle.drawShape();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.updateMvMatrix();
	myEngine.setFlatColor(0.25,0.25,0.25);
	trapeze.drawShape();
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({60.0,0.0,0.0});
		myEngine.mvMatrixStack.addHomothety(10.);
		myEngine.setFlatColor(0.25,0.5,0.5);
		myEngine.updateMvMatrix();
		cercle.drawShape();
	myEngine.mvMatrixStack.popMatrix();
}

void drawRoundedSquare() {
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addHomothety({0.8,1.0,1.0});
		myEngine.updateMvMatrix();
		myEngine.setFlatColor(0.5,0.25,0.25);
		carre.drawShape();
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addHomothety({1.0,0.8,1.0});
		myEngine.updateMvMatrix();
		myEngine.setFlatColor(0.5,0.25,0.25);
		carre.drawShape();
	myEngine.mvMatrixStack.popMatrix();

	float decal{0.4};
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({decal,decal,0.0});
		myEngine.mvMatrixStack.addHomothety(0.1);
		myEngine.updateMvMatrix();
		myEngine.setFlatColor(0.5,0.25,0.25);
		cercle.drawShape();
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({-decal,-decal,0.0});
		myEngine.mvMatrixStack.addHomothety(0.1);
		myEngine.updateMvMatrix();
		myEngine.setFlatColor(0.5,0.25,0.25);
		cercle.drawShape();
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({-decal,decal,0.0});
		myEngine.mvMatrixStack.addHomothety(0.1);
		myEngine.updateMvMatrix();
		myEngine.setFlatColor(0.5,0.25,0.25);
		cercle.drawShape();
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({decal,-decal,0.0});
		myEngine.mvMatrixStack.addHomothety(0.1);
		myEngine.updateMvMatrix();
		myEngine.setFlatColor(0.5,0.25,0.25);
		cercle.drawShape();
	myEngine.mvMatrixStack.popMatrix();

}

void drawSecondArm() {
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({25.0-5.0,0.0,0.0});
		myEngine.mvMatrixStack.addHomothety({46.0,6.0,1.0});
		myEngine.updateMvMatrix();
		myEngine.setFlatColor(0.25,0.5,0.25);
		carre.drawShape();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addHomothety(10);
		drawRoundedSquare();
	myEngine.mvMatrixStack.popMatrix();
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({50.0-10.0,0.0,0.0});
		myEngine.mvMatrixStack.addHomothety(10);
		drawRoundedSquare();
	myEngine.mvMatrixStack.popMatrix();
}

void drawThirdArm() {
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addHomothety(6.0);
		myEngine.updateMvMatrix();
		myEngine.setFlatColor(0.25,0.5,0.5);
		carre.drawShape();
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({20.0-2.0,0.0,0.0});
		myEngine.mvMatrixStack.addHomothety({40.0,4.0,1.0});
		myEngine.updateMvMatrix();
		myEngine.setFlatColor(0.5,0.5,0.5);
		carre.drawShape();
	myEngine.mvMatrixStack.popMatrix();

	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addTranslation({40.0-2.0,0.0,0.0});
		myEngine.mvMatrixStack.addHomothety(4.0);
		myEngine.updateMvMatrix();
		myEngine.setFlatColor(0.5,0.5,0.25);
		cercle.drawShape();
	myEngine.mvMatrixStack.popMatrix();

}

void renderScene() {
	glPointSize(4.0);

	//drawFirstArm(); // 01
	/* 02
	myEngine.mvMatrixStack.pushMatrix();
		myEngine.mvMatrixStack.addHomothety(60.0);
		drawRoundedSquare();
	myEngine.mvMatrixStack.popMatrix();
 	*/
	// 03
	//drawSecondArm();
	// 04
	drawThirdArm();
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
