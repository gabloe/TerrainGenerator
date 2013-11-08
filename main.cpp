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

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* obj.getNumVertices(), obj.getVertices(), GL_STATIC_DRAW);

	// Indexes
	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.getNumIndices() * sizeof(GLuint) , &obj.getIndices , GL_STATIC_DRAW );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	// Things!
	glDrawElements(obj.getDisplayMode(), obj.getNumIndices(), GL_UNSIGNED_INT, (void*)0);

}

int main(int argc , char** args)
{
   GLFWwindow* window;
   glfwSetErrorCallback(error_callback);
   if (!glfwInit())
      exit(EXIT_FAILURE);
   window = glfwCreateWindow(640, 480, "Happy Time Fun Time ALPHA v10E-100", NULL, NULL);
   if (!window)
   {
      glfwTerminate();
      exit(EXIT_FAILURE);
   }
   glfwMakeContextCurrent(window);
   glfwSetKeyCallback(window, key_callback);
   while (!glfwWindowShouldClose(window))
   {
      // Main Loop.  Do the stuff!
      glBegin(GL_LINE_LOOP);
      for(int i =0; i <= 300; i++){
         double angle = 2 * PI * i / 300;
         double x = cos(angle);
         double y = sin(angle);
         glVertex2d(x,y);
      }
      glEnd(); 
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   glfwDestroyWindow(window);
   glfwTerminate();
   exit(EXIT_SUCCESS);
}
