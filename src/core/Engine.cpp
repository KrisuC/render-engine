//
// Created by Krisu on 2020-02-05.
//

#include "Engine.hpp"


Engine::~Engine() {
    for (auto ps : scenes) {
        delete ps;
    }
    delete renderer;
}

Scene *Engine::createScene() {
    auto scene = new Scene;
    scenes.push_back(scene);
    return scene;
}

Renderer *Engine::getRenderer() {
    return renderer;
}

Engine::Engine() {
    renderer = new Renderer;
}

Engine *Engine::GetEngine() {
    static Engine *engine = new Engine();
    return engine;
}
