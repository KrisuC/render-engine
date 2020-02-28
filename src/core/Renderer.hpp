//
// Created by Krisu on 2019-12-29.
//

#ifndef RENDER_ENGINE_RENDERER_HPP
#define RENDER_ENGINE_RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameObject.hpp"
#include "Scene.hpp"


/*
 * Renderer encapsulates OpenGL loader things, OpenGL API and window asset
 * It's a middle layer between OpenGL API and the render core, i.e., a
 * platform-independent layer
 *
 * Usage:
 * 1. Render a whole scene
 *
 * Renderer renderer;
 * Scene scene;
 *
 * preparing scene...
 *
 * while (!Renderer.end()) {
 *      Renderer.draw(scene);
 * }
 *
 */

class GUIManager;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void setVsync(bool on);

    void setMSAA(int samples);

    bool end();

    void render(const Scene &scene);

    void render(const GameObject &gameObject) {

    }

    void initializeOpenGL();

private:
    void beforeRenderPass();

    void afterRenderPass();

private:
    GLFWwindow *window = nullptr;

    friend class GUIManager;
};


#endif //RENDER_ENGINE_RENDERER_HPP
