#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
    float pitch = -20.0f, yaw = -90.0f;
public:
    glm::vec3 Pos, Front, Up;
    glm::mat4 proj;

    Camera() { Reset(); }

    void Move(float dir_forward, float dir_right) {
        float speed = 0.8f;
        Pos += Front * dir_forward * speed;
        Pos += glm::normalize(glm::cross(Front, Up)) * dir_right * speed;
    }

    void Rotate(float p_offset, float y_offset) {
        float sensitivity = 1.5f;
        pitch += p_offset * sensitivity;
        yaw += y_offset * sensitivity;
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        Front = glm::normalize(direction);
    }

    void Reset() {
        Pos = {0, 60, 150}; Front = {0, -0.4f, -1.0f}; Up = {0, 1, 0};
        proj = glm::perspective(glm::radians(45.0f), 1200.0f/800.0f, 0.1f, 2000.0f);
    }

    glm::mat4 GetView() { return glm::lookAt(Pos, Pos + Front, Up); }
};
