#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <cmath>


#include "renderer\RenderObject.h"


#define PI 3.14159

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

	// Variables
	GLuint vertexBuffer;
	GLuint elementbuffer;

	// Data
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* obj.getNumVertices(), obj.getVertices(), GL_STATIC_DRAW);

	// Indexes
	glGenBuffers(1, &elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.getNumIndices() * sizeof(GLuint) , obj.getIndices() , GL_STATIC_DRAW );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	// Things!
	glDrawElements(obj.getDisplayMode(), obj.getNumIndices(), GL_UNSIGNED_INT, (void*)0);

}

int main(int argc, char** args)
{
	

	GLFWwindow* window;
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

   GLfloat data[] = {
	   -1.0f, -1.0f, 0.0f,
	   1.0f, -1.0f, 0.0f,
	   0.0f, 1.0f, 0.0f,
   };

   GLuint ind[] = { 0 , 1 , 2};

   RenderObject obj;

   obj.setVertices(data, 9);
   obj.setIndices(ind, 3);
   

   while (!glfwWindowShouldClose(window))
   {
      // Main Loop.  Do the stuff!
	  display(obj);
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   glfwDestroyWindow(window);
   glfwTerminate();
   exit(EXIT_SUCCESS);
}
