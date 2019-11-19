#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "shader.hpp"
#include "simpleWindow.hpp"
#include "helloTriangle.hpp"

int main() {
    SimpleWindow window(800, 600, "lab");

    Renderer *renderer = new TriangleTestRenderer();
    renderer->initContext();

    while (!window.shouldClose()) {
        window.processInput();

        renderer->render();

        window.swapBuffers();
        window.pollEvents();
    }
    return 0;
}

