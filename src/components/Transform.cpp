//
// Created by Krisu on 2019-12-09.
//

#include "Transform.hpp"
#include "Material.hpp"
#include "Scene.hpp"

void Transform::BeforeRenderPass() {
    auto& shader = GetGameObject().GetComponent<Material>().GetShader();
    auto& camera = Engine::GetEngine().GetCurrentScene().GetCurrentCamera();
    auto [w, h] = Engine::GetEngine().GetRenderer().GetWindowSize();
    glm::mat4 projection = glm::perspective(glm::radians(camera.GetFovy()),
            static_cast<float>(w)/h, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    glm::mat4 model = glm::scale(glm::mat4(1.0f), this->scale);
    model = glm::rotate(model, this->rotation_angle, this->rotation_axis);
    model = glm::translate(model, this->position);

    shader.UseShaderProgram();
    shader.SetTransform(projection, view, model);
}
