//
// Created by Krisu on 2019-10-27.
//

#ifndef RENDER_LAB_SIMPLEWINDOW_HPP
#define RENDER_LAB_SIMPLEWINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Adjust opengl size each time the window is resized
static void framebuffer_size_callback(GLFWwindow * window,
                                      int width, int height);

// This class encapsulate the glfw library into a pseudo OO manner
class SimpleWindow {
public:
    SimpleWindow(unsigned width, unsigned height,
           const std::string &title="window",
           unsigned v_major = 4,
           unsigned v_minor = 1);

    inline bool shouldClose() { return glfwWindowShouldClose(window); }
    inline float getTime() { return glfwGetTime(); }

    void processInput();
    void swapBuffers() { glfwSwapBuffers(window); }
    void pollEvents() { glfwPollEvents(); }
    ~SimpleWindow() { glfwTerminate(); }

private:
    GLFWwindow  *window;
};




#endif //RENDER_LAB_SIMPLEWINDOW_HPP
