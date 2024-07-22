#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include <GL/gl.h>
#include <iostream>

/* Window properties */
static const unsigned int WINDOW_WIDTH {800};
static const unsigned int WINDOW_HEIGHT {600};
static const char WINDOW_TITLE[] {"TD02 Ex02"};
static float aspectRatio {1.0};

/* Minimal time wanted between two images */
static const double FRAMERATE_IN_SECONDS {1. / 30.};

/* Virtual windows space */
static const float GL_VIEW_SIZE {2.};

/* Vertex structure */
typedef struct vertex_2D {
	float x;
	float y;
} Vertex2D;

/* Array of points */
#define NB_MAX_PT 100
static unsigned int nb_pt_clicked {0};
static Vertex2D clicked_pt[NB_MAX_PT];
static unsigned int type_dessin {1}; // 1 : Points 2 : Line strip

/* Error handling function */
void onError(int error, const char* description) {
	std::cout << "GLFW Error ("<<error<<") : " << description << std::endl;
}

void onWindowResized(GLFWwindow* /*window*/, int width, int height)
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

void onKey(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
	if (action == GLFW_PRESS) {
		switch(key) {
			case GLFW_KEY_A : glfwSetWindowShouldClose(window, GLFW_TRUE); break;
			case GLFW_KEY_L : type_dessin = 2; break;
			case GLFW_KEY_P : type_dessin = 1; break;
			default: std::cout<<"Touche non gérée "<<std::endl;
		}
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
		Vertex2D pt;

		/* 
		CONSIGNE TD : les deux équations ci-dessous à éventuellement donner en TD si les étudiants galèrent trop
		(-1 + 2. * pos_button.x / (float) width_window) * (GL_VIEW_SIZE / 2)
		( 1 - 2. * pos_button.y / (float) height_window) * (GL_VIEW_SIZE / 2)
		*/
		if (aspectRatio>1.0) {
			pt.x = (-1 + 2. * xpos / (float) s_w) * aspectRatio * (GL_VIEW_SIZE / 2);
			pt.y = (1 - 2. * ypos / (float) s_h) * (GL_VIEW_SIZE / 2);
		}
		else {
			pt.x = (-1 + 2. * xpos / (float) s_w) * (GL_VIEW_SIZE / 2);
			pt.y = (1 - 2. * ypos / (float) s_h) * (GL_VIEW_SIZE / 2) / aspectRatio;
		}
		clicked_pt[nb_pt_clicked++] = pt;
	}
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
		if (type_dessin == 1) glBegin(GL_POINTS);
		else glBegin(GL_LINE_STRIP);
		for(unsigned int i=0;i<nb_pt_clicked;i++) {
			glVertex2f(clicked_pt[i].x,clicked_pt[i].y);
		}
		glEnd();

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
