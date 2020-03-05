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
        for (GameObject & gameObject : gameObjects) {
            for (auto it : gameObject.componentsMap) {
                auto & component = it.second;
                component->BeforeRenderPass();
            }
        }
        for (auto& gobj : gameObjects) {
            renderer.Render(gobj);
        }
    }

    Camera& GetCamera() { return camera; }

private:
    std::vector<GameObject> gameObjects;
    std::vector<Light> lights;
    std::unique_ptr<Skybox> up_skybox;
    Camera camera {};

    friend class Renderer;
};




#endif //RENDER_ENGINE_SCENE_HPP
