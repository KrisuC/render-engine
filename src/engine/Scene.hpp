//
// Created by Krisu on 2020-02-05.
//

#ifndef RENDER_ENGINE_SCENE_HPP
#define RENDER_ENGINE_SCENE_HPP

#include <vector>
#include "basic/Renderable.hpp"
#include "basic/Light.hpp"
#include "Skybox.hpp"

/*
 * Scene holds all Renderables, Lights and some other resources in the scene
 */

class Scene {
public:
    Scene() = default;

    void setSkyBox(Skybox skybox_to_use) {
        skybox = std::move(skybox_to_use);
    }

    void addRenderable(const Renderable& renderable) {
        renderables.push_back(renderable);
    }

    void addLight(const Light& light) {
        lights.push_back(light);
    }

private:
    std::vector<Renderable> renderables;
    std::vector<Light> lights;
    Skybox skybox;
};


#endif //RENDER_ENGINE_SCENE_HPP
