//
// Created by Krisu on 2019-10-27.
//

#ifndef RENDER_LAB_APPLICATION_HPP
#define RENDER_LAB_APPLICATION_HPP

#include <GLFW/glfw3.h>

// The base class of App
class Application {
public:
    Application(int argc, char *argv[]);
    int exec();

private:
    void renderPass();
    void renderInit();
    void cleanUp();

private:
    bool applicationEnds = false;
    GLFWwindow *window;
};

#endif //RENDER_LAB_APPLICATION_HPP
