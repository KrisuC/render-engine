//
// Created by Krisu on 2019-11-19.
//

#ifndef RENDER_ENGINE_CAMERA_HPP
#define RENDER_ENGINE_CAMERA_HPP


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <algorithm>

namespace {
    const float YAW = -90.0f; // pointing to z
    const float PITCH = 0.0f;
    const float SPEED = 5.0f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;
}

class Camera {
public:
    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                    float yaw = YAW,
                    float pitch = PITCH);

    glm::mat4 GetViewMatrix();

    inline float GetFovy() const { return fovy; }

    inline glm::vec3 Front() const { return front; }
    inline glm::vec3 Up() const { return up; }
    inline glm::vec3 Right() const { return right; }

    void Translate(glm::vec3 const& direction);

    void ProcessMouseMovement(float xoffset, float yoffset,
                              GLboolean constrainPitch = true);

    void ProcessMouseScroll(float yoffset);

private:
    /* Calculates the front vector from the Camera's (updated) Eular Angles */
    void updateCameraVectors();

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    float yaw;
    float pitch;

    // camera options
    float movementSpeed;
    float mouseSensitivity;
    float fovy;
};

#endif //RENDER_ENGINE_CAMERA_HPP
