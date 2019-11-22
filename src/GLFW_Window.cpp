//
// Created by Krisu on 2019-11-18.
//

#include <GLFW_Window.hpp>
#include <iostream>

static void framebuffer_size_callback(GLFWwindow *window,
                                      int width, int height) {
    glViewport(0, 0, width, height);
}

GLFW_Window::GLFW_Window() {

    glfwInit();
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwMakeContextCurrent(window);
    
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        exit(-1);
    }
}

void GLFW_Window::createWindow() {
    window = glfwCreateWindow(mWidth, mHeight,
                              title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to initialize glfw window\n";
        glfwTerminate();
        exit(-1);
    }
}

void GLFW_Window::setGLVersion(int major, int minor) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
}

bool GLFW_Window::shouldClose() { return glfwWindowShouldClose(window); }

void GLFW_Window::close() { glfwSetWindowShouldClose(window, true); }

float GLFW_Window::getTime() { return glfwGetTime(); }

void GLFW_Window::swapBuffers() { glfwSwapBuffers(window); }

void GLFW_Window::pollEvents() { glfwPollEvents(); }

bool GLFW_Window::keyPress(int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

GLFW_Window::~GLFW_Window() { glfwTerminate(); }
