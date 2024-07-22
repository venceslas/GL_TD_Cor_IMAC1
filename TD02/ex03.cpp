#define GLFW_INCLUDE_NONE
#define _USE_MATH_DEFINES

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <GL/gl.h>
#include <iostream>
#include <cmath>

/* Window properties */
static const unsigned int WINDOW_WIDTH {800};
static const unsigned int WINDOW_HEIGHT {600};
static const char WINDOW_TITLE[] {"TD02 Ex03"};
static float aspectRatio {1.0};

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS {1. / 30.};

/* Virtual windows space */
// Point are defined in interval -0.5 et 0.5 on x and y axes
static const float GL_VIEW_SIZE {4.};

/* Vertex structure */
typedef struct vertex_2D {
	float x;
	float y;
} Vertex2D;

/* Scene flags */
#define NB_SEG_CIRCLE 64
static unsigned int is_full {0};
static unsigned int type_obj {0}; // 0 frame, 1 square, 2 circle

/* Error handling function */
void onError(int error, const char* description) {
	std::cout << "GLFW Error ("<<error<<") : " << description << std::endl;
}

void onWindowResized(GLFWwindow* /* window */, int width, int height)
{
	aspectRatio = width / (float) height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if( aspectRatio > 1)
	{
		glOrtho(
			-GL_VIEW_SIZE / 2. * aspectRatio, GL_VIEW_SIZE / 2. * aspectRatio,
			-GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.,
			-1.0,1.0
		);
	}
	else
	{
		glOrtho(
			-GL_VIEW_SIZE / 2., GL_VIEW_SIZE / 2.,
			-GL_VIEW_SIZE / 2. / aspectRatio, GL_VIEW_SIZE / 2. / aspectRatio,
			-1.0,1.0
		);
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

void onMouseButton(GLFWwindow* window, int button, int action, int /* mods */)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout<<"Pressed in "<<xpos<<" "<<ypos<<std::endl;
		int s_w,s_h;
		glfwGetWindowSize(window,&s_w,&s_h);
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
	glColor3f(1.0,1.0,1.0);
	if (is_full) glBegin(GL_TRIANGLE_FAN);
	else glBegin(GL_LINE_LOOP);
		glVertex2f(-0.5,-0.5);
		glVertex2f(0.5,-0.5);
		glVertex2f(0.5,0.5);
		glVertex2f(-0.5,0.5);
	glEnd();
}

void drawCircle() {
	glColor3f(1.0,1.0,0.0);
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

		glColor3f(1.0,0.0,1.0);
		switch(type_obj) {
			case 0 : drawOrigin(); break;
			case 1 : drawSquare(); break;
			case 2 : drawCircle(); break;
			default : std::cout<<"Wrong type of object "<<std::endl;
		}

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
