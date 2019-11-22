#include <Application.hpp>

int main(int argc, char *argv[]) {
    Application app(argc, argv);
    app.setSize(1280, 720);

    return app.exec();
}

