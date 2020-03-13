//
// Created by Krisu on 2019-12-09.
//

#include "Transform.hpp"
#include "Material.hpp"
#include "Scene.hpp"

glm::mat4 Transform::GetMatrix() {
    glm::mat4 model = glm::translate(glm::mat4(1), this->position);
    model = glm::rotate(model, this->rotation_angle, this->rotation_axis);
    model = glm::scale(model, this->scale);
    return model;
}


void Transform::BeforeRenderPass() {
    auto& shader = GetGameObject().GetComponent<Material>().GetShader();

    auto model = GetMatrix();

    shader.UseShaderProgram();
    shader.SetModel(model);

    auto& camera = Engine::GetInstance().GetCurrentScene().GetCurrentCamera();
    shader.SetCameraPosition(camera.Position());
}

void Transform::Translate(float dx, float dy, float dz) {
    this->position += glm::vec3{dx, dy, dz};
}

void Transform::SetPosition(glm::vec3 p) {
    this->position = p;
}

void Transform::SetPosition(float x, float y, float z) {
    this->position = {x, y, z};
}

void Transform::SetRotation(float axis_x, float axis_y, float axis_z,
                            float angle_in_degree) {
    this->rotation_axis = {axis_x, axis_y, axis_z};
    this->rotation_angle = glm::radians(angle_in_degree);
}

void Transform::SetRotation(glm::vec3 axis, float angle_in_degree) {
    this->rotation_axis = axis;
    this->rotation_angle = glm::radians(angle_in_degree);
}

void Transform::SetScale(float x, float y, float z) {
    this->scale = {x, y, z};
}

void Transform::SetScale(glm::vec3 s) {
    this->scale = s;
}

void Transform::SetScale(float factor) {
    this->scale = glm::vec3 {factor};
}

