//
// Created by Krisu on 2020-02-05.
//

#ifndef RENDER_ENGINE_ENGINE_HPP
#define RENDER_ENGINE_ENGINE_HPP

#include <vector>

#include "Scene.hpp"
#include "Renderer.hpp"
#include "Renderer.hpp"


/*
 * Engine hold all the Scene(s) and Component(s)
 */

class Engine {
public:
    static Engine* GetEngine();

    Engine(Engine const& e) = delete;
    Engine& operator=(Engine const& e) = delete;

    ~Engine();

    Scene* createScene();
    Renderer* getRenderer();

    template <typename T, typename... Args>
    T* create(Args&&...args) {
        T *obj = new T{std::forward<T>(args...)};
        return obj;
    }

private:
    Engine();

public:
    std::vector<Scene*> scenes;
    std::vector<Material*> materials;
    std::vector<Shader*> shaders;
    std::vector<Mesh*> meshes;
    Renderer* renderer;
};


#endif //RENDER_ENGINE_ENGINE_HPP
