//
// Created by Krisu on 2020-02-05.
//

#ifndef RENDER_ENGINE_ENGINE_HPP
#define RENDER_ENGINE_ENGINE_HPP

#include <vector>

#include "Renderer.hpp"
#include "Shader.hpp"

/*
 * Engine hold all the Scene(s) and Component(s)
 */

class Scene;

class Engine {
public:
    static Engine& GetInstance();

    Engine(Engine const&) = delete;
    Engine& operator=(Engine const&) = delete;

    Engine(Engine &&) = delete;
    Engine& operator=(Engine &&) = delete;

    ~Engine() = default;

    Scene& CreateScene();

    template <typename...Args>
    Shader& CreateShader(Args &&... args);

    Shader& GetDefaultShader();

    Shader& GetTestShader();

    Renderer& GetRenderer();

    Scene& GetScene(std::vector<Scene>::size_type index);

    void MakeCurrentScene(Scene& scene);

    Scene& GetCurrentScene();;

private:
    Engine();

private:
    std::vector<std::unique_ptr<Scene>> scenes;
    std::vector<std::unique_ptr<Shader>> shaders;

    Scene* currentScene = nullptr;
    Renderer renderer {};
};


#endif //RENDER_ENGINE_ENGINE_HPP
