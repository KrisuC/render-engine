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
 * It's a middle layer between OpenGL API and the Render core, i.e., a
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
 * while (!Renderer.End()) {
 *      Renderer.draw(scene);
 * }
 *
 */

class GUIManager;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void SetVsync(bool on);

    void SetMSAA(int samples);

    bool End();

    void Render(Scene const& scene);

    void Render(GameObject const& gameObject) {

    }

    void InitializeOpenGL();

private:
    void processKeyboard();

    void beforeRenderPass();

    void afterRenderPass();

private:
    GLFWwindow *window = nullptr;

    friend class GUIManager;
};


#endif //RENDER_ENGINE_RENDERER_HPP
