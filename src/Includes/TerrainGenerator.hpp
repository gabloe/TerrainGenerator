#pragma once

/**
 * TerrainGenerator.hpp
 * Contributors:
 *      * Gabriel Loewen
 * Licence:
 *      * MIT
 */

#ifndef OPENGL_CMAKE_TERRAINGENERATOR
#define OPENGL_CMAKE_TERRAINGENERATOR

#include <OGLApplication.hpp>

#include <Model.hpp>
#include <Shader.hpp>

#include <memory>
#include <ConfigReader.hpp>

#include <Camera.hpp>

class TerrainGenerator : public OGLApplication {
 public:
  TerrainGenerator(config::ConfigReader& configReader);
  camera::Camera camera;

  std::vector<std::shared_ptr<models::Model>> models;
  bool firstMouse = true;
  float mouseScale = 5.0f;
  float yaw = -90.0f;
  float pitch = 0.0f;
  float fov = 45.0;
  float znear = 0.1f;
  float zfar = 500.0f;

  glm::vec3 global_light_position = glm::vec3(2.0f, 2.0f, 2.0f);
  float light_rotation_timestamp = 0.0f;

 protected:
  virtual void render();
  virtual void mouseMoved(GLFWwindow*, double, double);
  virtual void mouseScroll(GLFWwindow*, double, double);
  virtual void handleKeyboardEvent(GLFWwindow*, int, int, int, int);

 private:
  void Init();
  const int size = 1024;
  size_t num_vertices;
  size_t num_indexes;

  // Model
  std::string modelPath;

  // shader
  std::unique_ptr<ShaderProgram> shaderProgram;

  std::string vertexShaderPath;
  std::string fragmentShaderPath;

  // shader matrix uniforms, start with identity
  glm::mat4 projection = glm::mat4(1.0);
  glm::mat4 view = glm::mat4(1.0);

  // VBO/VAO/ibo
  GLuint vao, vbo, ibo;

  // polygon representation
  GLenum polygonModes[2] = {GL_FILL, GL_LINE};
  int polygonMode = 0;
  bool polygonModePressed = false;

  // input handling
  void processInput(GLFWwindow*);
};

#endif  // OPENGL_CMAKE_TERRAINGENERATOR
