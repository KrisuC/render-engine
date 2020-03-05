//
// Created by Krisu on 2020-02-05.
//

#ifndef RENDER_ENGINE_SCENE_HPP
#define RENDER_ENGINE_SCENE_HPP

#include <vector>
#include "GameObject.hpp"
#include "Skybox.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Renderer.hpp"
#include "Engine.hpp"

/*
 * Scene holds all Renderables, Lights and some other asset in the scene
 *
 * Usage:
 * 1. create a simple scene with one light source, one renderable and Render
 *    the scene
 *
 * Scene scene;
 * Light light = ...;
 * GameObject renderable = ...;
 * Skybox skybox = ...;
 *
 * scene.AddLight(light);
 * scene.AddGameObject(renderable);
 * scene.addSkybox(skybox);
 * scene.generateSceneGraph();
 *
 * while (!Renderer.End()) {
 *      Renderer.renderScene(scene);
 * }
 *
 * TODO: Save/Load Scene in a JSON file
 */

class Renderer;

class Scene {
public:
    Scene() = default;

    Scene(Scene const&) = delete;
    Scene& operator=(Scene const&) = delete;

    Scene(Scene &&) = default;
    Scene& operator=(Scene &&) = default;

    GameObject& CreateGameObject();

    void AddLight(const Light &light);

    void SetSkybox(std::unique_ptr<Skybox> up_sb);

    /* Setting Shader component due to scene configuration */
    void Build();

    void Update() {
        auto& renderer = Engine::GetEngine().GetRenderer();

        for (GameObject & gameObject : gameObjects) {
            for (auto it : gameObject.componentsMap) {
                auto & component = it.second;
                component->BeforeRenderPass();
            }
        }
        for (auto& gameObject : gameObjects) {
            renderer.Render(gameObject);
        }
    }

    Camera& GetCurrentCamera() { return camera; }

private:
    std::vector<GameObject> gameObjects;
    std::vector<Light> lights;
    std::unique_ptr<Skybox> up_skybox;
    Camera camera {};

    friend class Renderer;
};




#endif //RENDER_ENGINE_SCENE_HPP
