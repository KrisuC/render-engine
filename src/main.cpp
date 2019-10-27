#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "shader.hpp"
#include "window_manage.hpp"
#include "triangleTestR.hpp"

int main() {
    Window window(800, 600, "render lab");

    Renderer *renderer = new TriangleTestR();
    renderer->initContext();

    // the 'render loop'
    int i = 0;
    while (!window.shouldClose()) {
        window.processInput();

        renderer->render();

        window.swapBuffers();
        window.pollEvents();
        i++;
    }
    return 0;
}

