//
// Created by Krisu on 2020-02-05.
//

#include "Scene.hpp"

void Scene::Build() {
    IBL const& ibl = up_skybox->GetIBL();
    for (auto& up_gameObject : up_gameObjects) {
        /* Check if it's renderable */
        try {
            auto& material = up_gameObject->GetComponent<Material>();
            auto& shader = material.GetShader();

            material.setIBLTextures(ibl);
            material.updateShaderUniform();

            shader.UseShaderProgram();
            for (int i = 0; i < lights.size(); i++) {
                shader.SetLight(lights[i], i);
            }
        } catch (NoComponent&) {
            // TODO: Change with better mechanism
            continue;
        }

        /* Call BeforeRenderLoop() for all components */
        for (auto it : up_gameObject->componentsMap) {
            auto & component = it.second;
            component->BeforeRenderLoop();
        }
    }
}

void Scene::CreateLight(const Light &light) {
    lights.push_back(light);
}

GameObject &Scene::CreateGameObject() {
    up_gameObjects.emplace_back(std::make_unique<GameObject>());
    return *up_gameObjects.back();
}

void Scene::Update() {

    auto& renderer = Engine::GetEngine().GetRenderer();
    for (auto& up_gameObject : up_gameObjects) {
        // BEFORE
        for (auto it : up_gameObject->componentsMap) {
            auto & component = it.second;
            component->BeforeRenderPass();
        }
        // RENDERING
        renderer.Render(*up_gameObject);
        up_skybox->Render();
        // AFTER
        for (auto it : up_gameObject->componentsMap) {
            auto & component = it.second;
            component->AfterRenderPass();
        }
    }
}
