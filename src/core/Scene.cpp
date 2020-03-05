//
// Created by Krisu on 2020-02-05.
//

#include "Scene.hpp"

void Scene::SetSkybox(std::unique_ptr<Skybox> up_sb) {
    up_skybox = std::move(up_sb);
}

void Scene::Build() {
    IBL const& ibl = up_skybox->GetIBL();
    for (auto& gameObject : gameObjects) {
        // Setting IBL
        try {
            auto& material = gameObject.GetComponent<Material>();
            auto& shader = material.GetShader();

            material.SetIBLTextures(ibl);
            material.UpdateShaderUniform();
            for (const auto& light : lights) {
                shader.SetLight(light);
            }
        } catch (NoComponent&) {
            continue;
        }
        // Updating all component
        for (auto it : gameObject.componentsMap) {
            auto & component = it.second;
            component->BeforeRenderLoop();
        }
    }
}

void Scene::AddLight(const Light &light) {
    lights.push_back(light);
}

void Scene::Update(Renderer const &renderer) {
    auto [w, h] = renderer.GetWindowSize();
    auto projection_mat = glm::perspective(
            glm::radians(camera.GetFovy()),
            static_cast<float>(w)/h, 0.1f, 100.0f);
    auto view_mat = camera.GetViewMatrix();

    for (auto& gobj : gameObjects) {
        /* Update Transform to Shader if GameObject has one */
        try {
            auto& transform = gobj.GetComponent<Transform>();
            auto& shader = gobj.GetComponent<Material>().GetShader();

            shader.SetModelTransform(transform);
            shader.SetProjectionView(projection_mat, view_mat);
        } catch (NoComponent&) {
            continue;
        }
    }
}

GameObject &Scene::CreateGameObject() {
    gameObjects.emplace_back();
    return gameObjects.back();
}
