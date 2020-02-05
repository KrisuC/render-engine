//
// Created by Krisu on 2019-12-29.
//

#ifndef RENDER_ENGINE_RENDERER_HPP
#define RENDER_ENGINE_RENDERER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "basic/Renderable.hpp"

/*
 * Renderer encapsulates OpenGL loader things and OpenGL API.
 * It's a middle layer between OpenGL API and the render engine, i.e., a
 * platform-independent layer
 */

class GUIManager;

class Renderer {
public:
    Renderer();

    void setVsync(bool on);

    void setMSAA(int samples);

private:
    GLFWwindow *window;

    friend class GUIManager;
};


#endif //RENDER_ENGINE_RENDERER_HPP
