#include <Camera.hpp>

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

void camera::Camera::HandleMouseScroll(float yoffset) {
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