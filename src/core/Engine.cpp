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



Engine::~Engine() {
    for (auto ps : scenes) {
        delete ps;
    }
    delete renderer;
}

Scene *Engine::CreateScene() {
    auto scene = new Scene;
    scenes.push_back(scene);
    return scene;
}

Renderer *Engine::GetRenderer() {
    return renderer;
}

Engine::Engine() {
    chdir("..");
    renderer = new Renderer;
}

Engine *Engine::GetEngine() {
    static auto *engine = new Engine();
    return engine;
}
