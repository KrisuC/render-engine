#include <fstream>
#include <sstream>
#include <iostream>

#include "Scene.hpp"
#include "Renderer.hpp"
#include "Engine.hpp"
#include "Mesh.hpp"
#include "Transform.hpp"
#include "Material.hpp"
#include "Debug.hpp"
#include "IO.hpp"

int main(int argc, char *argv[]) {
    /* Game code begins here */
    Engine *engine = Engine::GetEngine();

    Scene *scene = engine->CreateScene();
    scene->AddLight(PointLight({2, 2, 2},
                               {1, 1, 1}));

    GameObject object;
    object.CreateComponent<Mesh>(SimpleMesh::Sphere());

    auto& material = object.CreateComponent<Material>();
    material.SetShader(&Shader::TestShader());
    material.SetShader(&Shader::DefaultShader());
    material.SetAlbedo(1, 0, 0);
    material.SetMetallic(1.0);
    material.SetRoughness(0.2);
    auto& transform = object.CreateComponent<Transform>();
    transform.Translate(0, 0, -10);

    scene->AddGameObject(std::move(object));

    scene->SetSkybox(new Skybox);
    scene->Build();

    Renderer *renderer = engine->GetRenderer();
    while (!renderer->End()) {
        renderer->RenderScene(*scene);
    }


    /* Game code ends here */


    return 0;
}


