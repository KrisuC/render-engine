//
// Created by Krisu on 2019-10-27.
//

#ifndef RENDER_LAB_APPLICATION_HPP
#define RENDER_LAB_APPLICATION_HPP

#include <GLFW_Window.hpp>

class Application {
public:
    Application();
    Application(int argc, char *argv[]);
    ~Application();

    void setWindowSize(int w, int h);
    int exec();

protected:
    void renderPass();
    void renderInit();

protected:
    GLFW_Window window;
};

#endif //RENDER_LAB_APPLICATION_HPP
