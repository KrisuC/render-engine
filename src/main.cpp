#include <Application.hpp>

int main(int argc, char *argv[]) {
    Application::initializeContext();
    Application app(argc, argv);
    return app.exec();
}


