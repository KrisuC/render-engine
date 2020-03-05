//
// Created by Krisu on 2020-02-05.
//

#include "Scene.hpp"

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
        // Call BeforeRenderLoop() for all components
        for (auto it : gameObject.componentsMap) {
            auto & component = it.second;
            component->BeforeRenderLoop();
        }
    }
}

void Scene::CreateLight(const Light &light) {
    lights.push_back(light);
}

GameObject &Scene::CreateGameObject() {
    gameObjects.emplace_back();
    return gameObjects.back();
}

void Scene::Update() {
    // BEFORE
    for (GameObject & gameObject : gameObjects) {
        for (auto it : gameObject.componentsMap) {
            auto & component = it.second;
            component->BeforeRenderPass();
        }
    }
    // RENDERING
    auto& renderer = Engine::GetEngine().GetRenderer();
    for (GameObject & gameObject : gameObjects) {
        renderer.Render(gameObject);
    }
    up_skybox->Render();
    // AFTER
    for (GameObject & gameObject : gameObjects) {
        for (auto it : gameObject.componentsMap) {
            auto & component = it.second;
            component->AfterRenderPass();
        }
    }
}
