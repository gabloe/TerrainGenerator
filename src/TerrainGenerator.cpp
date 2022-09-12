/**
 * TerrainGenerator.cpp
 * Contributors:
 *      * Gabriel Loewen
 *      * Arthur Sonzogni (author)
 * Licence:
 *      * MIT
 */

#include "TerrainGenerator.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <iostream>
#include <vector>

#include "asset.hpp"
#include "glError.hpp"

struct VertexType {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec4 color;
};

float heightMap(const glm::vec2 position) {
  return 2.0 * sin(position.x) * sin(position.y);
}

VertexType getHeightMap(const glm::vec2 position) {
  const glm::vec2 dx(1.0, 0.0);
  const glm::vec2 dy(0.0, 1.0);

  VertexType v;
  float h = heightMap(position);
  float hx = 100.f * (heightMap(position + 0.01f * dx) - h);
  float hy = 100.f * (heightMap(position + 0.01f * dy) - h);

  v.position = glm::vec3(position, h);
  v.normal = glm::normalize(glm::vec3(-hx, -hy, 1.0));

  float c = sin(h * 5.f) * 0.5 + 0.5;
  v.color = glm::vec4(c, 1.0 - c, 1.0, 1.0);
  return v;
}

TerrainGenerator::TerrainGenerator()
    : OGLApplication(),
      vertexShader(SHADER_DIR "/shader.vert", GL_VERTEX_SHADER),
      fragmentShader(SHADER_DIR "/shader.frag", GL_FRAGMENT_SHADER),
      shaderProgram({vertexShader, fragmentShader}) {
  glCheckError(__FILE__, __LINE__);

  // creation of the mesh ------------------------------------------------------
  std::vector<VertexType> vertices;
  std::vector<GLuint> index;

  for (int y = 0; y <= size; ++y)
    for (int x = 0; x <= size; ++x) {
      float xx = (x - size / 2) * 0.1f;
      float yy = (y - size / 2) * 0.1f;
      vertices.push_back(getHeightMap({xx, yy}));
    }

  for (int y = 0; y < size; ++y)
    for (int x = 0; x < size; ++x) {
      index.push_back((x + 0) + (size + 1) * (y + 0));
      index.push_back((x + 1) + (size + 1) * (y + 0));
      index.push_back((x + 1) + (size + 1) * (y + 1));

      index.push_back((x + 1) + (size + 1) * (y + 1));
      index.push_back((x + 0) + (size + 1) * (y + 1));
      index.push_back((x + 0) + (size + 1) * (y + 0));
    }

  std::cout << "vertices=" << vertices.size() << std::endl;
  std::cout << "index=" << index.size() << std::endl;

  // creation of the vertex array buffer----------------------------------------

  // vbo
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexType),
               vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // ibo
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint),
               index.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // vao
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // bind vbo
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // map vbo to shader attributes
  shaderProgram.setAttribute("position", 3, sizeof(VertexType),
                             offsetof(VertexType, position));
  shaderProgram.setAttribute("normal", 3, sizeof(VertexType),
                             offsetof(VertexType, normal));
  shaderProgram.setAttribute("color", 4, sizeof(VertexType),
                             offsetof(VertexType, color));

  // bind the ibo
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  // vao end
  glBindVertexArray(0);

  registerKeypressCallbacks();

  // setup the camera
  cameraPos = glm::vec3(0.0, 0.0, 3.0);
  cameraFront = glm::vec3(0.0f, -1.0f, 0.0f);
  cameraUp    = glm::vec3(0.0f, 0.0f,  1.0f);
  cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
  cameraDirection = glm::normalize(cameraPos - cameraTarget);
}

void TerrainGenerator::registerKeypressCallbacks() {
  // register key press callbacks
  registerKeypressCallback(GLFW_KEY_W, std::function<void()>([&]() {
    cameraPos += cameraFront * speed;
  }));

  registerKeypressCallback(GLFW_KEY_S, std::function<void()>([&]() { 
    cameraPos -= cameraFront * speed;
  }));

  registerKeypressCallback(GLFW_KEY_A, std::function<void()>([&]() { 
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
  }));

  registerKeypressCallback(GLFW_KEY_D, std::function<void()>([&]() { 
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
  }));

  registerKeypressCallback(GLFW_KEY_ESCAPE, std::function<void()>([&]() { 
    glfwSetWindowShouldClose(getWindow(), GL_TRUE);
  }));
}

void TerrainGenerator::loop() {
  // exit on window close button pressed
  if (glfwWindowShouldClose(getWindow()))
    exit();

  double mouse_x,mouse_y;

	glfwGetCursorPos(getWindow(), &mouse_x, &mouse_y);

  // set matrix : projection + view
  projection = glm::perspective(glm::radians(fov), getWindowRatio(), 0.1f, 100.0f);  

  // glm::lookAt(eye, center, up)
  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

  // fire all the applicable keypress callbacks
  for (std::map<int, std::function<void()>>::iterator it = keypressCallbacks.begin(); it != keypressCallbacks.end(); it++) {
    if (glfwGetKey(getWindow(), it->first) != GLFW_RELEASE) {
      it->second();
    }
  }

  // clear
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shaderProgram.use();

  // send uniforms
  shaderProgram.setUniform("projection", projection);
  shaderProgram.setUniform("view", view);

  glCheckError(__FILE__, __LINE__);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

  glCheckError(__FILE__, __LINE__);
  glDrawElements(GL_TRIANGLES,         // mode
                 size * size * 2 * 3,  // count
                 GL_UNSIGNED_INT,      // type
                 NULL                  // element array buffer offset
  );

  glBindVertexArray(0);

  shaderProgram.unuse();
}

void TerrainGenerator::mouseMoved(GLFWwindow * window, double x, double y) {
  float xpos = static_cast<float>(x);
  float ypos = static_cast<float>(y);
  if (firstMouse)
  {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; 
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw   += xoffset;
  pitch += yoffset;

  if(pitch > 89.0f)
      pitch = 89.0f;
  if(pitch < -89.0f)
      pitch = -89.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * -cos(glm::radians(pitch));
  direction.z = sin(glm::radians(pitch));
  direction.y = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(direction);
}