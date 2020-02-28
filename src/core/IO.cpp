//
// Created by Krisu on 2020-02-05.
//

#include "IO.hpp"

namespace io {
    GLFWwindow *_current_glfw_window_ = nullptr;

    bool KeyPress(Key k) {
        return glfwGetKey(_current_glfw_window_, k) == GLFW_PRESS;
    }

    std::pair<double, double> MousePosition() {
        double xpos, ypos;
        glfwGetCursorPos(_current_glfw_window_, &xpos, &ypos);
        return std::make_pair(xpos, ypos);
    }
}
