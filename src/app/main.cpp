#if defined(WIN32) || defined(_WIN32)
    #include <direct.h>
    #define chdir _chdir
#else
    #include <unistd.h>
#endif

#include <fstream>
#include <sstream>
#include <iostream>


#include "Scene.hpp"
#include "Renderer.hpp"
#include "Engine.hpp"

int main(int argc, char *argv[]) {
    chdir("..");

    /* Game code begins here */
    Engine *engine = Engine::GetEngine();

    Scene *scene = engine->createScene();
    scene->addLight(PointLight({2, 2, 2},
                                     {1, 1, 1}));

    auto mesh = engine->create<Mesh>();
    auto material = engine->create<Material>();
    material->setAlbedo(0.9, 0, 0);
    material->setMetallic(0.1);
    material->setRoughness(0.2);

    auto default_shader = engine->create<Shader>();
    material->setShader(default_shader);

    scene->addGameObject(GameObject(mesh, material));

    scene->setSkybox(new Skybox);
    scene->build();

    Renderer *renderer = engine->getRenderer();
    while (!renderer->end()) {
        renderer->render(*scene);
    }


    /* Game code ends here */


    return 0;
}


