//
// Created by Krisu on 2019-11-18.
//

#include "glfw_window.hpp"
#include <iostream>

GLFW_Window::GLFW_Window(unsigned width, unsigned height,
                     const std::string &title,
                     unsigned int v_major, unsigned int v_minor) :
                     m_width(width), m_height(height) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, v_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, v_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(width, height,
                              title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to initialize glfw window\n";
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    auto framebuffer_size_callback = [](GLFWwindow *window,
                                        int width, int height) {
        glViewport(0, 0, width, height);
    };
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

//    // Disable cursor
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        exit(-1);
    }
}
