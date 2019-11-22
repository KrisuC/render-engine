//
// Created by Krisu on 2019-11-22.
//

#include <Application.hpp>
#include <Renderer.hpp>

Application::Application() : window() {}

Application::Application(int argc, char **argv) : window() {}

int Application::exec() {
    renderInit();
    // The render loop
    while (!window.shouldClose()) {
        renderPass();
    }
    return 0;
}


void Application::setWindowSize(int w, int h) {
    window.setSize(w, h);
}

Application::~Application() {

}

void Application::renderPass() {

}

void Application::renderInit() {

}
