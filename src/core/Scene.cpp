//
// Created by Krisu on 2020-02-05.
//

#include "Scene.hpp"

void Scene::SetSkybox(Skybox *sb) {
    if (!skybox) {
        delete skybox;
    }
    skybox = sb;
}

void Scene::Build() {
    for (auto& gobj : gameObjects) {
        try {
            auto& material = gobj.GetComponent<Material>();
            auto& shader = material.GetShader();

            material.UpdateShader();
            for (const auto& light : lights) {
                shader.SetLight(light);
            }
        } catch (no_component&) {
            continue;
        }
    }
}

void Scene::AddGameObject(GameObject &&gameObject) {
    gameObjects.push_back(std::move(gameObject));
}

void Scene::AddLight(const Light &light) {
    lights.push_back(light);
}
