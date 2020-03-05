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

Engine& Engine::GetEngine() {
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
