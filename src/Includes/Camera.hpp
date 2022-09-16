#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

namespace camera {

enum MovementDirection {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float DEFAULT_YAW         = -90.0f;
const float DEFAULT_PITCH       =  0.0f;
const float DEFAULT_SPEED       =  2.5f;
const float DEFAULT_SENSITIVITY =  0.1f;
const float DEFAULT_ZOOM        =  45.0f;

// Default vectors
const glm::vec3 DEFAULT_POSITION    = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 DEFAULT_UP          = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 DEFAULT_FRONT       = glm::vec3(0.0f, 0.0f, -1.0f);

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
        float MouseSensitivity;
        float Zoom;

        // constructors
        Camera();
        Camera(glm::vec3);
        Camera(glm::vec3, glm::vec3, glm::vec3, float, float, float, float, float);

        // public methods
        glm::mat4 GetViewMatrix();
        void HandleKeyboardInput(MovementDirection, float);
        void HandleMouseMovement(float, float, GLboolean);
        void HandleMouseScroll(float);

    private:
        void UpdateVectors();
};
} // namespace camera