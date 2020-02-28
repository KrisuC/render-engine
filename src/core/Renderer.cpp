//
// Created by Krisu on 2019-12-29.
//

#include "Renderer.hpp"

Renderer::Renderer() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    int width = 1280, height = 720;
    window = glfwCreateWindow(width, height, "Render core", nullptr, nullptr);
    if (!window) {
        throw std::runtime_error("GLFW window failed to create");
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        throw std::runtime_error("GLAD failed to load OpenGL");
    }
}


void Renderer::setVsync(bool on) {
    glfwSwapInterval(static_cast<int>(on));
}


void Renderer::setMSAA(int samples) {
    glfwWindowHint(GLFW_SAMPLES, samples);
    if (samples) {
        glEnable(GL_MULTISAMPLE);
    } else {
        glDisable(GL_MULTISAMPLE);
    }
}

Renderer::~Renderer() {
    glfwDestroyWindow(window);
    glfwTerminate();
}


