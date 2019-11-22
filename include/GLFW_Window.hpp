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
    GLFW_Window();
    GLFW_Window(const GLFW_Window &) = delete;
    GLFW_Window &operator=(const GLFW_Window &) = delete;
    ~GLFW_Window();

    inline void setGLVersion(int major, int minor);
    void createWindow();

    inline bool shouldClose();
    inline void close();
    inline float getTime();

    inline void swapBuffers();
    inline void pollEvents();

    inline unsigned width() { return mWidth; }
    inline unsigned height() { return mHeight; }

    inline bool keyPress(int key);


private:
    GLFWwindow  *window = nullptr;
    unsigned mWidth = 800, mHeight = 600;
    std::string title = "window";
};

#endif //RENDER_LAB_GLFW_WINDOW_HPP
