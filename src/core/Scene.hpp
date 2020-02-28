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

/*
 * Scene holds all Renderables, Lights and some other asset in the scene
 *
 * Usage:
 * 1. create a simple scene with one light source, one renderable and render
 *    the scene
 *
 * Scene scene;
 * Light light = ...;
 * GameObject renderable = ...;
 * Skybox skybox = ...;
 *
 * scene.addLight(light);
 * scene.addGameObject(renderable);
 * scene.addSkybox(skybox);
 * scene.generateSceneGraph();
 *
 * while (!Renderer.end()) {
 *      Renderer.renderScene(scene);
 * }
 *
 * TODO: Save/Load Scene in a JSON file
 */

class Renderer;

class Scene {
public:
    Scene() = default;

    void addGameObject(const GameObject& renderable) {
        gameObjects.push_back(renderable);
    }

    void addLight(const Light &light) {
        lights.push_back(light);
    }

    void setSkybox(Skybox *sb);

    void build() {
        // Setting Light to GameObject::Shader
        for (auto o : gameObjects) {
            for (auto light : lights) {

            }
        }
    }

private:
    std::vector<GameObject> gameObjects;
    std::vector<Light> lights;
    Skybox *skybox = nullptr;
    Camera *camera = nullptr;

    friend Renderer;
};


#endif //RENDER_ENGINE_SCENE_HPP
