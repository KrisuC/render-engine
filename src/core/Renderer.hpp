//
// Created by Krisu on 2019-12-29.
//

#ifndef RENDER_ENGINE_RENDERER_HPP
#define RENDER_ENGINE_RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameObject.hpp"


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

class Scene;

class GUIManager;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void SetVsync(bool on);

    void SetMSAA(int samples);

    bool End();

    void RenderScene(Scene& scene);

    void Render(GameObject& gameObject);

    void InitializeOpenGL();

    std::pair<int, int> GetWindowSize() const;

private:

    void beforeRenderPass();

    void afterRenderPass();

private:
    GLFWwindow *window = nullptr;

    friend class GUIManager;
};


#endif //RENDER_ENGINE_RENDERER_HPP
