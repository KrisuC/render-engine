//
// Created by Krisu on 2020-02-05.
//

#include "IO.hpp"

GLFWwindow *io::current_glfw_window = nullptr;

bool io::KeyPress(Key k) {
    return glfwGetKey(current_glfw_window, k) == GLFW_PRESS;
}

std::pair<double, double> io::MousePosition() {
    double xpos, ypos;
    glfwGetCursorPos(current_glfw_window, &xpos, &ypos);
    return std::make_pair(xpos, ypos);
}

void io::setCurrentWindow(GLFWwindow *window) {
    current_glfw_window = window;
}

