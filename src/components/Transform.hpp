//
// Created by Krisu on 2019-12-09.
//

#ifndef RENDER_ENGINE_TRANSFORM_HPP
#define RENDER_ENGINE_TRANSFORM_HPP

#include <glm/gtc/matrix_transform.hpp>

#include "Component.hpp"
#include "Engine.hpp"
#include "Camera.hpp"

class EulerAngle {
public:
    float pitch; // x
    float yaw;   // y
    float roll;  // z
};

class Transform : public Component {
public:
    /* Update all Transform of GameObject before InRenderPass called */
    void BeforeRenderPass() override;

public:
    Transform() = default;

    inline void SetPosition(float x, float y, float z) {
        this->position = {x, y, z};
    }

    inline void SetRotation(float axis_x, float axis_y, float axis_z,
            float angle_in_degree) {
        this->rotation_axis = {axis_x, axis_y, axis_z};
        this->rotation_angle = glm::radians(angle_in_degree);
    }

    inline void SetScale(float x, float y, float z) {
        this->scale = {x, y, z};
    }



private:
    glm::vec3 position {0, 0, 0};
    glm::vec3 rotation_axis {0, 1, 0};
    float rotation_angle {0};
    glm::vec3 scale {1, 1, 1};
};


#endif //RENDER_ENGINE_TRANSFORM_HPP
