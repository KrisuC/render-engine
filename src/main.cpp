#if defined(WIN32) || defined(_WIN32)
    #include <direct.h>
    #define chdir _chdir
#else
    #include <unistd.h>
#endif

#include "engine/Scene.hpp"
#include "engine/Renderer.hpp"

int main(int argc, char *argv[]) {
    chdir("..");

    /* Game code begins here */
    Scene scene;
    scene.addLight(PointLight({2, 2, 2}, {1, 1, 1}));
    auto *mesh = new Mesh(SimpleMesh::Sphere());
    auto *material = new Material;
    material->setAlbedo(0.9, 0, 0);
    material->setMetallic(0.1);
    material->setRoughness(0.2);
    auto *default_shader = new Shader;
    scene.addRenderable(Renderable(mesh, material, default_shader));
    scene.setSkybox(Skybox());
    scene.build();

    Renderer renderer;
    while (!renderer.end()) {
        renderer.draw(scene);
    }
    /* Game code ends here */

    return 0;
}


