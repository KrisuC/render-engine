//
// Created by Krisu on 2019-12-29.
//

#include <iostream>

#include "Renderer.hpp"
#include "IO.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include "Mesh.hpp"
#include "Material.hpp"

namespace {
    void ErrorCallBack(int error, const char* description) {
        std::cerr << "error code: " << error << std::endl;
        std::cerr << "error message: " << description << std::endl;
    }
}


Renderer::Renderer() {
    InitializeOpenGL();
}

void Renderer::InitializeOpenGL() {
    if (!glfwInit()) {
        std::cerr << "GLFW failed to init\n";
        exit(-1);
    }

    glfwSetErrorCallback(ErrorCallBack);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    int width = 1280, height = 720;
    window = glfwCreateWindow(width, height, "Render-engine", nullptr, nullptr);
    if (!window) {
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    io::setCurrentWindow(window);

    if (gladLoadGL() == 0) {
        std::cerr << "GLAD failed to init\n";
        exit(-1);
    }
}


void Renderer::SetVsync(bool on) {
    glfwSwapInterval(static_cast<int>(on));
}


void Renderer::SetMSAA(int samples) {
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

bool Renderer::ShouldEnd() {
    return glfwWindowShouldClose(window);
}

void Renderer::UpdateBeforeRendering() {
    glfwPollEvents();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::UpdateAfterRendering() {
    glfwSwapBuffers(window);
}

std::pair<int, int> Renderer::GetWindowSize() const {
    std::pair<int, int> size;
    glfwGetWindowSize(window, &size.first, &size.second);
    return size;
}

void Renderer::Render(GameObject& gameObject) {
    try {
        auto& mesh = gameObject.GetComponent<Mesh>();
        auto& material = gameObject.GetComponent<Material>();
        material.GetShader().UseShaderProgram();
        mesh.Draw();
    } catch(NoComponent&) {
        return ;
    }
}

double Renderer::GetDeltaTime() const {
    static double last_frame;
    auto this_frame = glfwGetTime();
    auto offset = this_frame - last_frame;
    last_frame = this_frame;
    return offset;
}

void Renderer::Close() {
    glfwSetWindowShouldClose(window, true);
}




