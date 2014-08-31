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
#include <list>

// Custom
#include "math/mat3.h"
#include "math/mat4.h"
#include "renderer/RenderObject.h"

float vert;
float horiz;

float delta_x = 0.1f;
float delta_y = 0.1f;

Mat4 TranslateMatrix = Mat4::LookAt(Vec3( 0, 1, 1), Vec3(0, 0, 0), Vec3(0, -1, 0));

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
			TranslateMatrix.moveZ(5.0f);
			break;
		case GLFW_KEY_S:
			TranslateMatrix.moveZ(-5.0f);
			break;
		case GLFW_KEY_D:
			TranslateMatrix.moveX(-0.5f);
			break;
		case GLFW_KEY_A:
			TranslateMatrix.moveX(0.5f);
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
			data[pos + 1] = 1.f - 50 * (rand() / float(RAND_MAX));
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
	return data;
}

void resized(GLFWwindow *window, int height, int width) {
	glViewport(0, 0, height, width);
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
	
	window = glfwCreateWindow(640, 480, "Terrain Generator", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, resized);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		glfwTerminate();
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		exit(EXIT_FAILURE);
	}glGetError();
	
	// Do some stuff
	glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
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
	init();print();
        TranslateMatrix.moveZ(-1000.0f);


	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Load the shader and compile it
	const std::string BaseShaderDir = std::string("../resources/shaders/");
	Shader shader(BaseShaderDir,std::string("shader"));

	if (shader.getError() == SHADER_ERROR::NO_SHADER_ERROR) {
		printf("No error loading shader\n");
	}else {
		printf("Error with shader\n");
	}
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	const Mat4 ProjectionMatrix = Mat4::Perspective(45.0f, (float)height / (float)width, -0.1f, 100.0f);
	
	const int divisions = 200;
	const int number_vertices = 3 * divisions * divisions;
	const int number_indicies = 6 * (divisions-1) * (divisions - 1);
	const float size = 100.0f;
	GLfloat *ground_data = generateGround(-size, size, -size, size, divisions);
	GLuint *indices = generateIndices(divisions);

	std::list<RenderObject> objs;
	objs.insert(
		objs.end(),
		RenderObject(shader, ground_data, number_vertices, indices, number_indicies)
	);

	// Main Loop.  Do the stuff!
	while (!glfwWindowShouldClose(window)) {
		// Clear everything on the screen
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		
		// for objects to be rendered
		for (RenderObject obj : objs) {
			obj.render(ProjectionMatrix, TranslateMatrix);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
