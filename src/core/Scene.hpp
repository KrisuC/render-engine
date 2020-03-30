//
// Created by Krisu on 2020-02-05.
//

#ifndef RENDER_ENGINE_SCENE_HPP
#define RENDER_ENGINE_SCENE_HPP

#include <vector>
#include <utility>


#include "Camera.hpp"
#include "Skybox.hpp"


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

class WindowManager;
class GameObject;

class Scene {
public:
    Scene() = default;

    Scene(Scene const&) = delete;
    Scene& operator=(Scene const&) = delete;

    Scene(Scene &&) = default;
    Scene& operator=(Scene &&) = default;

    GameObject& CreateGameObject();

    template <typename... Args> void CreateSkybox(Args... args);

    inline Skybox& GetSkybox() { return *up_skybox; }

    /* Setting Shader component due to scene configuration */
    void Build();

    void Update();

    inline Camera& GetCurrentCamera() { return camera; }

    auto& GetListOfObeject() { return up_game_objects; }

    GameObject* GetCurrentPointingGameObject(float viewport_x, float viewport_y) {

        return nullptr;
    }

private:
    void UpdateUniformBlocks();

private:
    /* Using a pointer to GameObject is a must because when vector change
       size. Reference of GameObject will fail!!! */
    std::vector<std::unique_ptr<GameObject>> up_game_objects;
    // std::vector<Light> lights;
    std::unique_ptr<Skybox> up_skybox;
    Camera camera {};

};


template<typename... Args>
void Scene::CreateSkybox(Args... args) {
    up_skybox = std::make_unique<Skybox>(args...);
}



#endif //RENDER_ENGINE_SCENE_HPP
