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

GameObject &Scene::CreateGameObject() {
    gameObjects.emplace_back();
    return gameObjects.back();
}
