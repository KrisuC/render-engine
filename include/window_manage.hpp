//
// Created by Krisu on 2019-10-27.
//

#ifndef RENDER_LAB_WINDOW_MANAGE_HPP
#define RENDER_LAB_WINDOW_MANAGE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Adjust opengl size each time the window is resized
static void framebuffer_size_callback(GLFWwindow * window,
                                      int width, int height);
// This class encapsulate the glfw library into a pseudo OO manner
class Window {
public:
    Window(unsigned width, unsigned height, const std::string &title="window",
           unsigned v_major = 4, unsigned v_minor = 1) {
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
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        // Load glad
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD\n";
            exit(-1);
        }
    }
    bool shouldClose() {
        return glfwWindowShouldClose(window);
    }
    void processInput() {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }
    float getTime() { return glfwGetTime(); }
    void swapBuffers() { glfwSwapBuffers(window); }
    void pollEvents() { glfwPollEvents(); }
    ~Window() {
        glfwTerminate();
    }

private:
    GLFWwindow  *window;
};

static void framebuffer_size_callback(GLFWwindow * window,
        int width, int height) {
    glViewport(0, 0, width, height);
}




#endif //RENDER_LAB_WINDOW_MANAGE_HPP
