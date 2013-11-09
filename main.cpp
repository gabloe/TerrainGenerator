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

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

// Variables
GLuint vertexBuffer;
GLuint elementbuffer;



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

// used to display a RenderObject
void display(RenderObject obj) {

	ShaderProgram *p = obj.getShaderProgram();
	if (p) {
		p->load();
	}

	GLenum mode = obj.getDisplayMode();
	GLuint num = obj.getNumIndices();
	glBindBufferARB(GL_ARRAY_BUFFER, vertexBuffer);	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat), BUFFER_OFFSET(0));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, elementbuffer);
	glDrawElements( mode , num , GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	glBindBufferARB(GL_ARRAY_BUFFER, 0 );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

	if (p) {
		p->unload();
	}
}

// Initializes all the subsystems, create the window.
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

	// Do some stuff
	glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);


}

// Just prints OpenGL information
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

	// Data
	glGenBuffersARB(1, &vertexBuffer);
	glBindBufferARB(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(float)* obj.getNumVertices(), obj.getVertices(), GL_STATIC_DRAW_ARB);

	// Indexes
	glGenBuffersARB(1, &elementbuffer);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, obj.getNumIndices() * sizeof(GLuint), obj.getIndices(), GL_STATIC_COPY_ARB);
	

	ShaderProgram program("../resources/shaders/shader.vert", "../resources/shaders/shader.frag");

	glEnableClientState(GL_VERTEX_ARRAY);
	

	if (program.getError() != SHADER_ERROR::NO_SHADER_ERROR ) {
		printf("No Error!\n");
	}

	obj.setShaderProgram(&program);

	// Main Loop.  Do the stuff!
	while (!glfwWindowShouldClose(window))
	{	

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		display(obj);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffersARB(1, &vertexBuffer);
	glDeleteBuffersARB(1, &elementbuffer);

	glUseProgram(0);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
