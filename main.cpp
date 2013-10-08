#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

static void error_callback(int error, const char* description)
{
   fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
   GLFWwindow* window;
   glfwSetErrorCallback(error_callback);
   if (!glfwInit())
      exit(EXIT_FAILURE);
   window = glfwCreateWindow(640, 480, "Happy Time Fun Time ALPHA v0.000000000000000000000000001", NULL, NULL);
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
      glfwSwapBuffers(window);
      glfwPollEvents();
   }
   glfwDestroyWindow(window);
   glfwTerminate();
   exit(EXIT_SUCCESS);
}
