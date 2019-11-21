#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "shader.hpp"
#include "glfw_window.hpp"

// Test cases:
#include "helloTriangle.hpp"
#include "pbrMetalBall.hpp"

int main() {
    GLFW_Window window(800, 600, "lab");

    Renderer *renderer = new PbrRenderer(window);
//    Renderer *renderer = new TriangleTestRenderer();

    while (!window.shouldClose()) {
        renderer->render();
    }
    return 0;
}

