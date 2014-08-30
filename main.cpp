#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <cmath>

#include <iostream>

#include "math\mat3.h"
#include "math\mat4.h"
#include "renderer\RenderObject.h"

#include <stdio.h>  /* defines FILENAME_MAX */
/*

projMatrix[0] = f / ratio;
projMatrix[1 * 4 + 1] = f;
projMatrix[2 * 4 + 2] = (farP + nearP) / (nearP - farP);
projMatrix[3 * 4 + 2] = (2.0f * farP * nearP) / (nearP - farP);
projMatrix[2 * 4 + 3] = -1.0f;
projMatrix[3 * 4 + 3] = 0.0f;

*/

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

float delta_x = 0.01;
float delta_y = 0.01;

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

Mat4 buildProjectionMatrix(float fov, float ratio, float nearP, float farP) {

	float f = 1.0f / tan(fov * (3.14159265359f / 360.0));

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
			TransformMatrix.moveZ(0.1);
			break;
		case GLFW_KEY_S:
			TransformMatrix.moveZ(-0.1);
			break;
		case GLFW_KEY_D:
			TransformMatrix.moveX(-0.01);
			break;
		case GLFW_KEY_A:
			TransformMatrix.moveX(0.01);
			// move right
			break;
		case GLFW_KEY_P:
			mode = (mode + 1) % 2;
			glPolygonMode(GL_FRONT_AND_BACK, MODES[mode]);
		}
	}
}

// used to display a RenderObject
void display(RenderObject renderObj) {
	GLuint transformMatrix = 0, projectionMatrix = 0;
	ShaderProgram *p = renderObj.getShaderProgram();
	if (p) {
		p->load();
		transformMatrix = glGetUniformLocation(p->getProgram(), "transform");
		projectionMatrix = glGetUniformLocation(p->getProgram(), "projection");
		glUniformMatrix4fv(transformMatrix, 1, false, TransformMatrix.getData());
		glUniformMatrix4fv(projectionMatrix, 1, false, ProjectionMatrix.getData());
	}

	GLenum mode = renderObj.getDisplayMode();
	GLuint num = renderObj.getNumIndices();
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	// Indexed Arrays
	if( num ) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glDrawElements( mode , num , GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	}else {// No index
		glDrawArrays(mode, 0, renderObj.getNumVertices() / 3);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0 );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (p) {
		p->unload();
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

	/*
	for (int i = 0; i < 3 * div * div; i++) {
		if (i % 3 == 0 && i > 0) {
			std::cout << std::endl;
		}
		std::cout << data[i] << " ";
	}
	std::cout << std::endl;
	// */

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

	ProjectionMatrix = buildProjectionMatrix( 15.f , 9.0f /16.0f , 0.001f , 100.f);

	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(640, 480, "Terrain Generator", NULL, NULL);
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

	// load data

	GLfloat data[] = {
		-0.5f,	-0.1f,	0.5f,	// 0
		0.5f,	-0.1f,	0.5f,	// 1
		-0.5f,	0.1f,	-0.5f,	// 2
		0.5f,	0.1f,	-0.5f// 3
	};
	GLuint ind[] = { 0, 1, 3 , 0 , 3 , 2 };
	RenderObject obj;

	int ground_size = 50;

	RenderObject ground;

	int vertex_size = ground_size * ground_size * 3;
	int ind_size = (ground_size - 1) * (ground_size - 1) * 6;

	ground.setVertices( generateGround(-30, 30, -30, 30 , ground_size ) , vertex_size );
	ground.setIndices( generateIndices(ground_size) , ind_size );
	ground.setMode(GL_TRIANGLES);

	obj.setVertices(data, 12);
	obj.setIndices(ind, 6);
	obj.setMode(GL_TRIANGLES);

	// Data
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* ground.getNumVertices(), ground.getVertices(), GL_STATIC_DRAW);

	// Indexes
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ground.getNumIndices() * sizeof(GLuint), ground.getIndices(), GL_STATIC_COPY);
	

	ShaderProgram program("../resources/shaders/shader.vert", "../resources/shaders/shader.frag");

	std::cout << program.isValid() << std::endl;

	glEnableClientState(GL_VERTEX_ARRAY);
	
	if (program.getError() == SHADER_ERROR::NO_SHADER_ERROR ) {
		printf("No Error!\n");
	}
	else{
		printf("Error!\n");
	}

	obj.setShaderProgram(&program);
	ground.setShaderProgram(&program);

	int i = 0;
	// Main Loop.  Do the stuff!
	while (!glfwWindowShouldClose(window))
	{	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		display(ground);

		glfwSwapBuffers(window);
		glfwPollEvents();
		i++;
	}

	free(ground.getVertices());
	free(ground.getIndices());

	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &elementbuffer);

	glUseProgram(0);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
