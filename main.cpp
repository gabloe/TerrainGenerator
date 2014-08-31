#define GLFW_DLL

// OpenGL
#include "GL/glew.h"
#include <GLFW/glfw3.h>

// C
#include <cstdlib>
#include <cstdio>
#include <cmath>

// C++
#include <iostream>

// Custom
#include "math/mat3.h"
#include "math/mat4.h"
#include "renderer/RenderObject.h"

Mat4 ProjectionMatrix(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
	);

Mat4 TransformMatrix (
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
	);

float vert;
float horiz;

float delta_x = 0.01f;
float delta_y = 0.01f;

#ifdef _WIN32
	#include <direct.h>
	#define GetCurrentDir _getcwd
#else
	#include <unistd.h>
#define GetCurrentDir getcwd
#endif

// The current window
GLFWwindow *window;

#define checkGL() {							\
	GLenum err = glGetError();				\
	if (err != GL_NO_ERROR) {				\
		std::cout << err << ": " <<			\
			gluErrorString(err) <<			\
			" at line " << __LINE__ <<		\
			" in file " << __FILE__ <<		\
			std::endl;						\
		std::exit(-1);						\
	}										\
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

Mat4 buildProjectionMatrix(float fov, float ratio, float nearP, float farP) {

	float f = (float)(1.0f / tan(fov * (3.14159265359f / 360.0)));

	return Mat4(f / ratio, 0, 0, 0,
		0, f, 0, 0,
		0, 0, (farP + nearP) / (nearP - farP), -1.0f,
		0, 0, (2.0f * farP * nearP) / (nearP - farP), 0);
}

GLenum MODES[2] = {GL_FILL , GL_LINE };
int mode = 0;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_REPEAT || action== GLFW_PRESS){
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_UP:
			vert += delta_x;
			break;
		case GLFW_KEY_DOWN:
			vert -= delta_x;
			break;
		case GLFW_KEY_LEFT:
			horiz += delta_y;
			break;
		case GLFW_KEY_RIGHT:
			horiz -= delta_y;
			break;
		case GLFW_KEY_W:
			TransformMatrix.moveZ(0.1f);
			break;
		case GLFW_KEY_S:
			TransformMatrix.moveZ(-0.1f);
			break;
		case GLFW_KEY_D:
			TransformMatrix.moveX(-0.01f);
			break;
		case GLFW_KEY_A:
			TransformMatrix.moveX(0.01f);
			// move right
			break;
		case GLFW_KEY_P:
			mode = (mode + 1) % 2;
			glPolygonMode(GL_FRONT_AND_BACK, MODES[mode]);
		}
	}
}

float* generateGround(float min_x, float max_x, float min_z, float max_z, int div) {
	
	float x_len = max_x - min_x;
	float z_len = max_z - min_z;
	float delta_x = x_len / (div - 1);
	float delta_z = z_len / (div - 1);
	float* data = (float*)calloc(3 * div * div, sizeof(float));
	
	for ( int i = 0; i < div; i++) { // z
		float z = max_z - i * delta_z;
		for (int j = 0 ; j < div; j++) { // x
			int pos = 3 * i * div + 3 * j;
			data[pos] = min_x + j * delta_x;
			data[pos + 1] = 0.f - rand() / float(RAND_MAX);
			data[pos + 2] = z;
		}
	}
	return data;
}

GLuint* generateIndices(int div) {
	GLuint* data = (GLuint*)calloc( (div - 1) * (div - 1) * 6, sizeof(GLuint));
	int max_i, max_j;
	max_i = max_j = div - 1;

	int A = 0;
	int B = 1;
	int C = div;
	int D = div + 1;

	int i = 0;
	for ( ; i < max_i; i++){
		int j = 0;
		for ( ; j < max_j; j++) {
			
			int pos = 6 * ( i * (div - 1) + j);

			// Create indices
			// Triangle One
			data[pos] = A;			// 0
			data[pos + 1] = B;		// 1
			data[pos + 2] = D;		// 3

			// Triangle Two
			data[pos + 3] = A;		// 0
			data[pos + 4] = D;		// 3
			data[pos + 5] = C;		// 2
		
			A++;
			B++;
			C++;
			D++;

		}

		A++;
		B++;
		C++;
		D++;


	}

	/*
	for (int i = 0; i < max_i * max_j * 6; i++){
		if (i > 0 && i % 3 == 0){
			std::cout << std::endl;
		}
		std::cout << data[i] << " ";
	}
	std::cout << std::endl;
	// */

	return data;
}

// Initializes all the subsystems, create the window.
void init() {

	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	int major, minor, rev;

	glfwGetVersion(&major, &minor, &rev);

	std::cerr << "GLFW sversion recieved: " << major << ", " << minor << ", " << rev << std::endl;
	window = glfwCreateWindow(640, 480, "Terrain Generator", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		glfwTerminate();
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		exit(EXIT_FAILURE);
	}glGetError();
	
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	// Do some stuff
	glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
}

// Just prints OpenGL information
void print() {
	char current[FILENAME_MAX];

	if (GetCurrentDir(current, sizeof(current))){
		printf("Current Working Directory : %s\n" , current );
	}

	printf("OpenGL Vendor: %s\n", glGetString(GL_VENDOR));
	printf("OpenGL Vendor: %s\n", glGetString(GL_RENDERER));
	printf("OpenGL Vendor: %s\n", glGetString(GL_VERSION));

}

int main(int argc, char** args)
{
	init();
	print();

	// Square Data
	GLfloat data[] = {
		-0.5f,	-0.1f,	0.5f,	// 0
		0.5f,	-0.1f,	0.5f,	// 1
		-0.5f,	0.1f,	-0.5f,	// 2
		0.5f,	0.1f,	-0.5f	// 3
	};
	GLuint ind[] = {
		0, 1, 3,	// T1
		0, 3, 2		// T2
	};

	// Load the shader and compile it
	ShaderProgram program("../resources/shaders/shader.vert", "../resources/shaders/shader.frag");

	if (program.getError() == SHADER_ERROR::NO_SHADER_ERROR) {
		printf("No error loading shader\n");
	}else {
		printf("Error with shader\n");
	}

	// Create our object
	RenderObject obj;
	obj.setVertices(data, 12);
	obj.setIndices(ind, 6);
	obj.setMode(GL_TRIANGLES);
	obj.setShaderProgram(&program);
	obj.moveToGPU();

	// Main Loop.  Do the stuff!
	while (!glfwWindowShouldClose(window))
	{	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		obj.render();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
