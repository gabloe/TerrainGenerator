#pragma once

#include <array>
#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

namespace camera {

enum MovementDirection { FORWARD, BACKWARD, LEFT, RIGHT };

// Percentage of default speed
enum MovementType {
  WALK = 150,     // 150%
  JOG = 500,      // 500%
  RUN = 1500,     // 1500%
  SPRINT = 3000,  // 3000%
  DEFAULT
};

constexpr const char* MovementTypeToString(MovementType t) {
  switch (t) {
    case MovementType::WALK:
      return "Walk";
    case MovementType::JOG:
      return "Jog";
    case MovementType::RUN:
      return "Run";
    case MovementType::SPRINT:
      return "Sprint";
    case MovementType::DEFAULT:
      return "Default";
    default:
      throw std::invalid_argument("Unimplemented item");
  };
};

// Ensure the elements are sorted descending
constexpr const std::array<MovementType, 4> AllMovementTypes = {
    MovementType::SPRINT, MovementType::RUN, MovementType::JOG,
    MovementType::WALK};

// Default camera values
const float DEFAULT_YAW = -90.0f;
const float DEFAULT_PITCH = 0.0f;
const float DEFAULT_SPEED = 2.5f;
const float DEFAULT_SENSITIVITY = 0.1f;
const float DEFAULT_ZOOM = 45.0f;

// Movement speed ladder threshold
const float MOVEMENT_SPEED_DELTA_T_STEP_THRESHOLD =
    0.01f;  // Increase the step when time delta since the last incease is above
            // the threshold

// Default vectors
const glm::vec3 DEFAULT_POSITION = glm::vec3(0.0f, 1.0f, 5.0f);
const glm::vec3 DEFAULT_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 DEFAULT_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);

class Camera {
 public:
  // camera Attributes
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // euler angles
  float Yaw;
  float Pitch;
  // camera options
  float MovementSpeed;
  int MovementTypeIndex = 0;
  int SpeedThreshold;
  float MouseSensitivity;
  float Zoom;
  float MovementSpeedDeltaT = 0;
  int MovementSpeedStep = 0;

  // constructors
  Camera();
  Camera(glm::vec3);
  Camera(glm::vec3, glm::vec3, glm::vec3, float, float, float, float, float);

  // public methods
  glm::mat4 GetViewMatrix();
  void HandleKeyboardInput(MovementDirection, float);
  void HandleMouseMovement(float, float, GLboolean);
  void HandleMouseScroll(float);

  // Control the rate of movement speed increase by step up over time delta
  void UpdateMovementSpeedStep(int, float);
  void ResetMovementSpeedStep();

 private:
  void UpdateVectors();
};
}  // namespace camera