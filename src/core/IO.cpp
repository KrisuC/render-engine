//
// Created by Krisu on 2020-02-05.
//

#include "IO.hpp"

GLFWwindow *io::current_glfw_window = nullptr;

bool io::KeyPress(Key k) {
    return glfwGetKey(current_glfw_window, static_cast<int>(k)) == GLFW_PRESS;
}

MousePos io::GetMousePosition() {
    double xpos, ypos;
    glfwGetCursorPos(current_glfw_window, &xpos, &ypos);
    return {xpos, ypos};
}

void io::setCurrentWindow(GLFWwindow *window) {
    current_glfw_window = window;
}

bool io::MouseButtonClick(MouseButton m) {
    return glfwGetMouseButton(current_glfw_window,  static_cast<int>(m));
}

