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

void processInput(Camera& camera) {
    float speed = 20.0 * Engine::GetEngine().GetRenderer().GetDeltaTime();
    if (io::KeyPress(Key::w)) {
        camera.Translate(camera.Front() *  speed);
    }
    if (io::KeyPress(Key::s)) {
        camera.Translate(camera.Front() * -speed);
    }
    if (io::KeyPress(Key::a)) {
        camera.Translate(camera.Right() * -speed);
    }
    if (io::KeyPress(Key::d)) {
        camera.Translate(camera.Right() * speed);
    }
    if (io::KeyPress(Key::escape)) {
        Engine::GetEngine().GetRenderer().Close();
    }

    MousePos current = io::GetMousePosition();
    static MousePos last = current;
    if (io::MouseButtonClick(MouseButton::right)) {
        MousePos offset = current - last;
        camera.ProcessMouseMovement(offset.x, offset.y);
    }
    last = current;

}

int main(int argc, char *argv[]) {
    /* Game code begins here */
    Engine& engine = Engine::GetEngine();

    Scene& scene = engine.CreateScene();
    scene.AddLight(PointLight({2, 2, 2},
                               {1, 1, 1}));

    GameObject& object = scene.CreateGameObject();
    object.CreateComponent<Mesh>(SimpleMesh::Sphere());

    auto& material = object.CreateComponent<Material>();
//    material.SetShader(&Shader::TestShader());
    material.SetShader(&Shader::DefaultShader());
    material.SetAlbedo(1, 0, 0);
    material.SetMetallic(1.0);
    material.SetRoughness(0.2);
    auto& transform = object.CreateComponent<Transform>();
    transform.SetPosition(0, 0, -10);

    scene.SetSkybox(std::make_unique<Skybox>());
    scene.Build();

    engine.MakeCurrentScene(scene);

    Renderer& renderer = engine.GetRenderer();
    while (!renderer.ShouldEnd()) {
        processInput(scene.GetCurrentCamera());
        scene.Update();
    }


    /* Game code ends here */


    return 0;
}


