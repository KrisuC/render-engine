//
// Created by Krisu on 2020-02-05.
//

#if defined(WIN32) || defined(_WIN32)
#include <direct.h>
    #define chdir _chdir
#else
#include <unistd.h>
#endif

#include "Engine.hpp"
#include "Scene.hpp"

Scene& Engine::CreateScene() {
    scenes.emplace_back(std::make_unique<Scene>());
    return *scenes.back();
}

Renderer& Engine::GetRenderer() {
    return renderer;
}

Engine::Engine() {
    /* Setting asset directory */
    chdir("..");
}


Engine& Engine::GetInstance() {
    static Engine engine;
    return engine;
}


Scene &Engine::GetScene(std::vector<Scene>::size_type index) {
    return *scenes[index];
}


Scene &Engine::GetCurrentScene() {
    assert(currentScene != nullptr);
    return *currentScene;
}


void Engine::MakeCurrentScene(Scene &scene) {
    currentScene = &scene;
}

template<typename... Args>
Shader &Engine::CreateShader(Args &&... args) {
    auto up_shader = std::make_unique<Shader>(std::forward<Args>(args)...);
    Shader& ret = *up_shader;
    shaders.push_back(std::move(up_shader));
    /* binding all enabled uniform blocks to newly created shader */
    for (auto & uniformBlock : uniformBlocks) {
        if (uniformBlock->enabled) {
            uniformBlock->BindShader(ret);
        }
    }
    return ret;
}


Shader &Engine::GetDefaultShader() {
    static Shader& default_ = CreateShader("shader/default.vert",
                                           "shader/default.frag");
    return default_;
}


Shader &Engine::GetTestShader() {
    static Shader& test = CreateShader("shader/test.vert", "shader/test.frag");
    return test;
}


void Engine::CreateUniformBlock(const std::string &uniform_name, int bytes) {
    static int current_uniform_bind_id = 0;

    auto p_new_uniform_block = std::make_unique<UniformBlock>(bytes, current_uniform_bind_id, uniform_name);
    current_uniform_bind_id++;
    uniformBlocks.push_back(std::move(p_new_uniform_block));
}


void Engine::EnableUniformBlock(const std::string &uniform_name) {

    findUniformBlockAndF(uniform_name,
            [](UniformBlock& ub) -> UniformBlock& { ub.enabled = true; return ub; });
}


void Engine::DisableUniformBlock(const std::string &uniform_name) {
    findUniformBlockAndF(uniform_name,
            [](UniformBlock& ub) -> UniformBlock& { ub.enabled = false; return ub; });
}


UniformBlock& Engine::findUniformBlockAndF(const std::string &uniform_name, const UBop& f) {
    for (auto & uniformBlock : uniformBlocks) {
        if (uniformBlock->uniform_block_name == uniform_name) {
            return f(*uniformBlock);
        }
    }
    throw std::runtime_error("No such Uniform Block exists");
}


UniformBlock& Engine::GetUniformBlock(const std::string &uniform_name) {
    return findUniformBlockAndF(uniform_name,
            [](UniformBlock& ub) -> UniformBlock& { return ub; });
}
