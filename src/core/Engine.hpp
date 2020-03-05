//
// Created by Krisu on 2020-02-05.
//

#ifndef RENDER_ENGINE_ENGINE_HPP
#define RENDER_ENGINE_ENGINE_HPP

#include <vector>

#include "Renderer.hpp"


/*
 * Engine hold all the Scene(s) and Component(s)
 */

class Engine {
public:
    static Engine& GetEngine();

    Engine(Engine const&) = delete;
    Engine& operator=(Engine const&) = delete;

    Engine(Engine &&) = delete;
    Engine& operator=(Engine &&) = delete;

    ~Engine() = default;

    Scene& CreateScene();
    Renderer& GetRenderer();

    Scene& GetScene(std::vector<Scene>::size_type index);

    void MakeCurrentScene(Scene& scene) {
        currentScene = &scene;
    }

    Scene& GetCurrentScene() {
        assert(currentScene != nullptr);
        return *currentScene;
    };

private:
    Engine();

private:
    std::vector<Scene> scenes;
    Scene* currentScene = nullptr;
    Renderer renderer{};
};


#endif //RENDER_ENGINE_ENGINE_HPP
