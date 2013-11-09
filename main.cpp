#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <cmath>

#include "renderer\RenderObject.h"

#include <stdio.h>  /* defines FILENAME_MAX */


#ifdef _WIN32
	#include <direct.h>
	#define GetCurrentDir _getcwd
#else
	#include <unistd.h>
#define GetCurrentDir getcwd
#endif


GLFWwindow *window;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void display(RenderObject obj) {

	// Things!
	glDrawElements(obj.getDisplayMode(), obj.getNumIndices(), GL_UNSIGNED_INT, (void*)0);

}

void init() {

	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(640, 480, "Happy Time Fun Time ALPHA v10E-100", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		glfwTerminate();
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glewInit()));
		system("PAUSE");
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

}


void print() {
	char current[FILENAME_MAX];

	if (GetCurrentDir(current, sizeof(current))){
		printf("%s\n" , current );
	}
}


int main(int argc, char** args)
{

	print();
	init();


	printf("OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
	printf("OpenGL Vendor: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL Vendor: %s\n", glGetString(GL_VERSION));



	GLfloat data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};
	GLuint ind[] = { 0, 1, 2 };
	RenderObject obj;

	obj.setVertices(data, 9);
	obj.setIndices(ind, 3);
	obj.setMode(GL_TRIANGLES);

	// Variables
	GLuint vertexBuffer;
	GLuint elementbuffer;

	// Data
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* obj.getNumVertices(), obj.getVertices(), GL_STATIC_DRAW);

	// Indexes
	glGenBuffers(1, &elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.getNumIndices() * sizeof(GLuint), obj.getIndices(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	ShaderProgram program("../resources/shaders/shader.vert", "../resources/shaders/shader.frag");

	if (program.getError() != SHADER_ERROR::NO_SHADER_ERROR ) {
		printf("No Error!\n");
	}

	// Main Loop.  Do the stuff!
	while (!glfwWindowShouldClose(window))
	{	
		glClearColor(1, 0, 0, 0 );
		glClear(GL_COLOR_BUFFER_BIT);

		display(obj);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glUseProgram(0);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
