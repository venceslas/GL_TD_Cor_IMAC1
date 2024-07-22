#define GLFW_INCLUDE_NONE
#define _USE_MATH_DEFINES

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <GL/gl.h>
#include <iostream>
#include <cmath>

/* Window properties */
static const unsigned int WINDOW_WIDTH {1000};
static const unsigned int WINDOW_HEIGHT {800};
static const char WINDOW_TITLE[] {"TD02 Ex04"};
static float aspectRatio {1.0};

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS {1. / 30.};

/* Virtual windows space */
// Point are defined in interval -3 et 3 on x and y axes
static const float GL_VIEW_SIZE {6.};

/* Vertex structure */
typedef struct vertex_2D {
	float x;
	float y;
} Vertex2D;

/* Scene flags */
#define NB_SEG_CIRCLE 128
static unsigned int is_full {0};
static unsigned int type_obj {0}; // 0 frame, 1 square, 2 circle
static Vertex2D square_center;
static float square_angle {0.0};

/* Error handling function */
void onError(int error, const char* description) {
	std::cout << "GLFW Error ("<<error<<") : " << description << std::endl;
}

void onWindowResized(GLFWwindow* /*window*/, int width, int height)
{
	aspectRatio = {width / (float) height};

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	std::cout<<"We resize with "<<aspectRatio<<std::endl;
	if( aspectRatio > 1)
	{
		glOrtho(
		-(GL_VIEW_SIZE / 2.) * aspectRatio, (GL_VIEW_SIZE / 2.) * aspectRatio,
		-GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.,
		-1.0,1.0);
	}
	else
	{
		glOrtho(
		-GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.,
		-GL_VIEW_SIZE / 2. / aspectRatio, GL_VIEW_SIZE / 2. / aspectRatio,
		-1.0,1.0);
	}
	glMatrixMode(GL_MODELVIEW);
}


void onKey(GLFWwindow* window, int key, int /* scancode */, int action, int /* mods */)
{
	if (action == GLFW_PRESS) {
		switch(key) {
			case GLFW_KEY_A :
			case GLFW_KEY_ESCAPE :
				glfwSetWindowShouldClose(window, GLFW_TRUE); 
				break;
			case GLFW_KEY_O : 
				type_obj = (type_obj+1)%3;
				std::cout<<"Forme "<<type_obj<<std::endl;
				break;
			case GLFW_KEY_F : 
				is_full = 1-is_full;
				break;
			default: std::cout<<"Touche non gérée "<<std::endl;
		}
	}
}

Vertex2D getClickedPt(GLFWwindow* window) {
	Vertex2D res;
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	int s_w,s_h;
	glfwGetWindowSize(window,&s_w,&s_h);
	std::cerr<<"aspect ratio "<<aspectRatio<<std::endl;

	if (aspectRatio>1.0) {
		res.x = (-GL_VIEW_SIZE / 2.0) * aspectRatio + (xpos / (float) s_w) * aspectRatio * GL_VIEW_SIZE;
		res.y = (GL_VIEW_SIZE / 2.0) - (ypos / (float) s_h) * GL_VIEW_SIZE;
	}
	else {
		res.x = (-GL_VIEW_SIZE / 2.0) +  (xpos / (float) s_w) * GL_VIEW_SIZE;
		res.y = (GL_VIEW_SIZE / 2.0)/ aspectRatio - (ypos / (float) s_h) * GL_VIEW_SIZE / aspectRatio;
	}
	return res;
}

void onMouseButton(GLFWwindow* window, int button, int action, int /* mods */)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		square_center = getClickedPt(window);
	}
}

void drawOrigin() {
	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex2f(0.0,0.0);
		glVertex2f(1.0,0.0);
		glColor3f(0.0,1.0,0.0);
		glVertex2f(0.0,0.0);
		glVertex2f(0.0,1.0);
	glEnd();
}

void drawSquare() {
	if (is_full) glBegin(GL_TRIANGLE_FAN);
	else glBegin(GL_LINE_LOOP);
		glVertex2f(-0.5,-0.5);
		glVertex2f(0.5,-0.5);
		glVertex2f(0.5,0.5);
		glVertex2f(-0.5,0.5);
	glEnd();
}

void drawCircle() {
	if (is_full) glBegin(GL_TRIANGLE_FAN);
	else glBegin(GL_LINE_LOOP);
		float step_rad = 2*M_PI/(float)NB_SEG_CIRCLE;
		for(int i=0;i<NB_SEG_CIRCLE;i++) {
			glVertex2f(cos(i*step_rad),sin(i*step_rad));
		}
	glEnd();
}

int main(int /* argc */, char** /* argv */) 
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

	// Intialize glad (loads the OpenGL functions)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return -1;
	}

	glfwSetWindowSizeCallback(window,onWindowResized);
	glfwSetKeyCallback(window, onKey);
	glfwSetMouseButtonCallback(window, onMouseButton);

	onWindowResized(window,WINDOW_WIDTH,WINDOW_HEIGHT);

	glPointSize(4.0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Get time (in second) at loop beginning */
		double startTime = glfwGetTime();

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW_MATRIX);
		glLoadIdentity();

		drawOrigin();

		// Partie 03
		glTranslatef(1.0,2.0,0.0);
		glColor3f(0.8,0.8,0.0);
		drawCircle();

		// Partie 04
		glLoadIdentity();
		glRotatef(45.0,0,0,1.0);
		glTranslatef(1.,0.,0.);
		glColor3f(1.0,0.0,0.0);
		drawSquare();

		// Partie 05
		glLoadIdentity();
		glTranslatef(1.,0.,0.);
		glRotatef(45.0,0,0,1.0);
		glColor3f(1.0,0.0,1.0);
		drawSquare();

		// Partie 06 : ordre des transformations...

		// Partie 07
		glColor3f(0.0,0.8,0.0);
		glLoadIdentity();
		glTranslatef(square_center.x,square_center.y,0.);
		// Partie 08
		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		if (state == GLFW_PRESS) {
			Vertex2D clkPt = getClickedPt(window);
			square_angle = atan((clkPt.y - square_center.y)/(clkPt.x - square_center.x));
			glRotatef(180.0*square_angle/M_PI,0,0,1);
		}
		drawSquare();

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
