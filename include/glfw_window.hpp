//
// Created by Krisu on 2019-10-27.
//

#ifndef RENDER_LAB_GLFW_WINDOW_HPP
#define RENDER_LAB_GLFW_WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>

// This class encapsulate the glfw library into a pseudo OO manner
class GLFW_Window {
public:
    GLFW_Window(unsigned width, unsigned height,
           const std::string &title="window",
           unsigned v_major = 4,
           unsigned v_minor = 1);
    GLFW_Window(const GLFW_Window &) = delete;
    GLFW_Window &operator=(const GLFW_Window &) = delete;

    inline bool shouldClose() { return glfwWindowShouldClose(window); }
    inline void close() { glfwSetWindowShouldClose(window, true); }
    inline float getTime() { return glfwGetTime(); }

    inline void swapBuffers() { glfwSwapBuffers(window); }
    inline void pollEvents() { glfwPollEvents(); }
    ~GLFW_Window() { glfwTerminate(); }

    inline unsigned width() { return m_width; }
    inline unsigned height() { return m_height; }

    inline bool keyPress(int key) { return glfwGetKey(window, key); }

private:
    GLFWwindow  *window;
    unsigned m_width, m_height;
};




#endif //RENDER_LAB_GLFW_WINDOW_HPP
