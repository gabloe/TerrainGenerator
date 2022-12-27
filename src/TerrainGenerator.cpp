/**
 * TerrainGenerator.cpp
 * Contributors:
 *      * Gabriel Loewen
 * Licence:
 *      * MIT
 */

#include <TerrainGenerator.hpp>

#include <GLFW/glfw3.h>
#include <vector>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <ResourceManager.hpp>

#include <ConfigReader.hpp>

#include <Asset.hpp>
#include <Logger.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

struct VertexType {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec4 color;
};

TerrainGenerator::TerrainGenerator(config::ConfigReader& configReader)
    : OGLApplication(),
      modelPath(asset::Asset::Instance().MODELS_DIR + "/tree.DAE"),
      vertexShaderPath(asset::Asset::Instance().SHADERS_DIR + "/shader.vert"),
      fragmentShaderPath(asset::Asset::Instance().SHADERS_DIR + "/shader.frag"),
      camera{glm::vec3(0.0f, 0.0f, 1.0f)} {
  if (configReader.ContainsKey("model")) {
    std::string modelName = configReader.ReadString("model");
    modelPath = asset::Asset::Instance().MODELS_DIR + "/" + modelName;
    logging::Logger::LogInfo("Overriding default model value: " + modelPath);
  }

  if (configReader.ContainsKey("vertexShader")) {
    std::string vertexShaderName = configReader.ReadString("vertexShader");
    vertexShaderPath =
        asset::Asset::Instance().SHADERS_DIR + "/" + vertexShaderName;
    logging::Logger::LogInfo("Overriding default vertex value: " +
                             vertexShaderPath);
  }

  if (configReader.ContainsKey("fragmentShader")) {
    std::string fragmentShaderName = configReader.ReadString("fragmentShader");
    fragmentShaderPath =
        asset::Asset::Instance().SHADERS_DIR + "/" + fragmentShaderName;
    logging::Logger::LogInfo("Overriding default fragment value: " +
                             fragmentShaderPath);
  }

  // setup the camera
  camera = camera::Camera();

  Init();
}

void TerrainGenerator::Init() {
  auto manager = resources::ResourceManager::GetManager();

  // Create shaders
  auto vertexShader = Shader(vertexShaderPath, GL_VERTEX_SHADER);
  auto fragmentShader = Shader(fragmentShaderPath, GL_FRAGMENT_SHADER);
  shaderProgram = std::make_unique<ShaderProgram>(
      std::initializer_list<Shader>{vertexShader, fragmentShader});

  auto model = manager.LoadModel(modelPath);

  models.push_back(model);
}

void TerrainGenerator::render() {
  // exit on window close button pressed
  if (glfwWindowShouldClose(getWindow()))
    exit();

  processInput(getWindow());

  // set matrix : projection + view
  projection = glm::perspective(glm::radians(camera.Zoom), getWindowRatio(),
                                znear, zfar);

  // glm::lookAt(eye, center, up)
  view = camera.GetViewMatrix();

  // clear
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  shaderProgram->use();

  // Rotate the light position by a small amount
  if (getTime() - light_rotation_timestamp >= 1.0f / 1000.0f) {
    light_rotation_timestamp = getTime();
    global_light_position = glm::rotateY(global_light_position, 1.0f / 360.0f);
  }

  // send uniforms
  shaderProgram->setUniform("camera", camera.Position);
  shaderProgram->setUniform("projection", projection);
  shaderProgram->setUniform("view", view);
  shaderProgram->setUniform("global_light_position", global_light_position);

  for (size_t i = 0; i < this->models.size(); i++) {
    this->models[i]->Draw(*shaderProgram);
  }

  shaderProgram->unuse();
}

void TerrainGenerator::mouseMoved(GLFWwindow* window, double x, double y) {
  OGLApplication::mouseMoved(window, x, y);
  const float offsetScale = mouseScale * 100.0f;

  auto newX = static_cast<float>(x) / getWidth() - 0.5f;
  auto newY = static_cast<float>(y) / getHeight() - 0.5f;

#ifdef __linux__
  static float xPosition = 0.5;
  static float yPosition = 0.5;

  auto offsetX = newX - xPosition;
  auto offsetY = yPosition - newY;
#else
  auto offsetX = newX;
  auto offsetY = -newY;
#endif

  camera.HandleMouseMovement(offsetScale * offsetX, offsetScale * offsetY,
                             true);

#ifdef __linux__
  xPosition = newX;
  yPosition = newY;
#endif
}

void TerrainGenerator::handleKeyboardEvent(GLFWwindow* window,
                                           int key,
                                           int scancode,
                                           int action,
                                           int mods) {
  OGLApplication::handleKeyboardEvent(window, key, scancode, action, mods);
}

void TerrainGenerator::mouseScroll(GLFWwindow* window,
                                   double xoffset,
                                   double yoffset) {
  OGLApplication::mouseScroll(window, xoffset, yoffset);
  camera.HandleMouseScroll(static_cast<float>(yoffset));
}

void TerrainGenerator::processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.HandleKeyboardInput(camera::MovementDirection::FORWARD,
                               getFrameDeltaTime());
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.HandleKeyboardInput(camera::MovementDirection::BACKWARD,
                               getFrameDeltaTime());
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.HandleKeyboardInput(camera::MovementDirection::LEFT,
                               getFrameDeltaTime());
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.HandleKeyboardInput(camera::MovementDirection::RIGHT,
                               getFrameDeltaTime());
  }

  // If shift is pressed we speed up
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
      glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
    camera.UpdateMovementSpeedStep(50, getFrameDeltaTime());
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
      glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_RELEASE) {
    camera.ResetMovementSpeedStep();
  }

  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
    setFullScreen(!isFullScreen());
  }

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    exit();
  }

  if (polygonModePressed && glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
    polygonModePressed = false;
    polygonMode = (polygonMode + 1) % 2;
    glPolygonMode(GL_FRONT_AND_BACK, polygonModes[polygonMode]);
  } else {
    polygonModePressed = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    camera.HandleMouseMovement(-1, 0, true);
  }

  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    camera.HandleMouseMovement(+1, 0, true);
  }
}