#include <Camera.hpp>
#include <Logger.hpp>
#include <algorithm>

using namespace camera;

// Constructors
Camera::Camera()
    : Position(DEFAULT_POSITION),
      WorldUp(DEFAULT_UP),
      Front(DEFAULT_FRONT),
      MovementSpeed(DEFAULT_SPEED),
      MouseSensitivity(DEFAULT_SENSITIVITY),
      Zoom(DEFAULT_ZOOM),
      Yaw(DEFAULT_YAW),
      Pitch(DEFAULT_PITCH) {

  UpdateVectors();
}

Camera::Camera(glm::vec3 position)
    : WorldUp(DEFAULT_UP),
      Front(DEFAULT_FRONT),
      MovementSpeed(DEFAULT_SPEED),
      MouseSensitivity(DEFAULT_SENSITIVITY),
      Zoom(DEFAULT_ZOOM),
      Yaw(DEFAULT_YAW),
      Pitch(DEFAULT_PITCH) {

  Position = position;
  UpdateVectors();
}

Camera::Camera(glm::vec3 position,
                glm::vec3 up,
                glm::vec3 front,
                float speed,
                float sensitivity,
                float zoom,
                float yaw,
                float pitch) {

  Position = position;
  UpdateVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::HandleKeyboardInput(MovementDirection direction, float deltaTime) {
  float velocity = MovementSpeed * deltaTime;
  if (direction == FORWARD) {
    Position += Front * velocity;
  }
  if (direction == BACKWARD) {
    Position -= Front * velocity;
  }
  if (direction == LEFT) {
    Position -= Right * velocity;
  }
  if (direction == RIGHT) {
    Position += Right * velocity;
  }
}

void Camera::HandleMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  Yaw   += xoffset;
  Pitch += yoffset;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch) {
    if (Pitch > 89.0f) {
      Pitch = 89.0f;
    }
    if (Pitch < -89.0f) {
      Pitch = -89.0f;
    }
  }

  // Update the vectors using updated euler angles
  UpdateVectors();
}

void Camera::HandleMouseScroll(float yoffset) {
  Zoom -= yoffset;
  if (Zoom < 1.0f) {
    Zoom = 1.0f;
  }
  if (Zoom > 45.0f) {
    Zoom = 45.0f; 
  }
}

void Camera::UpdateVectors(){
  // Recalculate the front vector
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

  // Make sure to normalize
  Front = glm::normalize(front);

  // Recalculate the right vector
  Right = glm::normalize(glm::cross(Front, WorldUp));

  // Recalculate the up vector
  Up    = glm::normalize(glm::cross(Right, Front));
}

void Camera::UpdateMovementSpeedStep(int increase, float deltaT) {
  // Don't step up if we are already at the highest step
  if (MovementSpeedStep >= *std::max_element(AllMovementTypes.begin(), AllMovementTypes.end())) {
    return;
  }

  // Increase the time delta until we hit the step up threshold
  MovementSpeedDeltaT += deltaT;

  if (MovementSpeedDeltaT >= MOVEMENT_SPEED_DELTAT_STEP_THRESHOLD) {
    // Reset the speed time delta and test whether movement speed should increase
    MovementSpeedDeltaT = 0;
    MovementSpeedStep += increase; // Update the speed % increase
    const MovementType movementTypeTest = static_cast<MovementType>(MovementSpeedStep);
    MovementType newMovementType = MovementType::DEFAULT;

    // Test if the new movement speed is at or above one of the steps
    for (auto t : AllMovementTypes) {
      if (movementTypeTest >= t) {
        newMovementType = t;
        logging::Logger::LogDebug("MovementType: " + std::string(MovementTypeToString(t)) + " (" + std::to_string(t) + "% / " + std::to_string(MovementSpeedStep) + "%)");
        break;
      }
    }

    // If we have stepped up to the next speed, get the new percentage increase and multiply by the base speed
    if (newMovementType != MovementType::DEFAULT) {
      MovementSpeed = newMovementType / 100.0f * DEFAULT_SPEED;
    }
  }
}

void Camera::ResetMovementSpeedStep() {
  MovementSpeedDeltaT = 0;
  MovementSpeedStep = 0;
  MovementSpeed = DEFAULT_SPEED;
  logging::Logger::LogDebug("MovementType reset");
}