//
// Created by Krisu on 2019-12-09.
//

#ifndef RENDER_ENGINE_TRANSFORM_HPP
#define RENDER_ENGINE_TRANSFORM_HPP

#include <glm/gtc/matrix_transform.hpp>

#include "Component.hpp"
#include "Engine.hpp"

class EulerAngle {
public:
    float pitch; // x
    float yaw;   // y
    float roll;  // z
};

class Transform : public Component {
public:
    /* Update all Transform of GameObject before render called */
    void Update() override;

public:
    Transform() = default;

    void Translate(float dx, float dy, float dz);

    void SetPosition(glm::vec3 p);

    void SetPosition(float x, float y, float z);

    void SetRotation(float axis_x, float axis_y, float axis_z,
            float angle_in_degree);

    void SetRotation(glm::vec3 axis, float angle_in_degree);

    void SetScale(float x, float y, float z);

    void SetScale(glm::vec3 s);

    void SetScale(float factor);

    glm::mat4 GetMatrix();

private:
    glm::vec3 position {0, 0, 0};
    /* store angle in radians */
    glm::vec3 rotation_axis {0, 1, 0};
    float rotation_angle {0};
    glm::vec3 scale {1, 1, 1};
};


#endif //RENDER_ENGINE_TRANSFORM_HPP
