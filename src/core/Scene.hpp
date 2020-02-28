//
// Created by Krisu on 2020-02-05.
//

#ifndef RENDER_ENGINE_SCENE_HPP
#define RENDER_ENGINE_SCENE_HPP

#include <vector>
#include "Renderable.hpp"
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
 * Renderable renderable = ...;
 * Skybox skybox = ...;
 *
 * scene.addLight(light);
 * scene.addRenderable(renderable);
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

    void setSkybox(const Skybox &box) {
        skybox = box;
    }

    void addRenderable(const Renderable& renderable) {
        renderables.push_back(renderable);
    }

    void addLight(const Light& light) {
        lights.push_back(light);
    }

    void build() {
        // Setting Light to Renderable::Shader
        for (auto renderable : renderables) {
            for (auto light : lights) {

            }
        }
    }

private:
    std::vector<Renderable> renderables;
    std::vector<Light> lights;
    Skybox skybox;
    Camera camera;

    friend Renderer;
};


#endif //RENDER_ENGINE_SCENE_HPP
