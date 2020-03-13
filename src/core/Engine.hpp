//
// Created by Krisu on 2020-02-05.
//

#ifndef RENDER_ENGINE_ENGINE_HPP
#define RENDER_ENGINE_ENGINE_HPP

#include <vector>
#include <functional>
#include <type_traits>
#include <unordered_map>

#include "Renderer.hpp"
#include "Shader.hpp"
#include "UniformBlock.hpp"
#include "GlobalTransform.hpp"
#include "LightInformation.hpp"

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

    Scene& GetCurrentScene();

    template <typename T>
    void EnableUniformBuffer();

    template <typename T>
    void DisableUniformBuffer();

    template <typename T>
    T& GetUniformBuffer();

private:
    /* Managing Uniform Block */
    template <typename T> void setUniformBlockEnabled(bool enabled);


    Engine();

private:
    std::vector<std::unique_ptr<Scene>> scenes;
    std::vector<std::unique_ptr<Shader>> shaders;
    /* size_t for storing type_info::hash_code */
    std::unordered_map<
        size_t, std::unique_ptr<UniformBlock>
    > uniform_blocks;

    Scene* currentScene = nullptr;
    Renderer renderer {};

    friend Scene;
};



template<typename T>
void Engine::EnableUniformBuffer() {
    setUniformBlockEnabled<T>(true);
}

template<typename T>
void Engine::DisableUniformBuffer() {
    setUniformBlockEnabled<T>(false);
}

template<typename T>
void Engine::setUniformBlockEnabled(bool enabled) {
    static_assert(std::is_base_of_v<UniformBlock, T>,
                  "uniform block must be derived of UniformBlock");
    auto hash = typeid(T).hash_code();
    auto it = uniform_blocks.find(hash);
    if (it == uniform_blocks.end()) { // not exist
        auto p = static_cast<UniformBlock*>(new T);
        p->SetEnabled(enabled);
        auto up = std::unique_ptr<UniformBlock>(p);
        uniform_blocks.insert({hash, std::move(up)});
    } else {
        it->second->SetEnabled(enabled);
    }
}

template<typename T>
T& Engine::GetUniformBuffer() {
    static_assert(std::is_base_of_v<UniformBlock, T>,
                  "uniform block must be derived of UniformBlock");
    auto it = uniform_blocks.find(typeid(T).hash_code());
    if (it == uniform_blocks.end()) {
        throw std::runtime_error("uniform block not found");
    }
    return *static_cast<T*>(it->second.get());
}



#endif //RENDER_ENGINE_ENGINE_HPP
