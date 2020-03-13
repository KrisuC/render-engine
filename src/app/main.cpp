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
#include "LightInformation.hpp"
#include "GlobalTransform.hpp"

void processInput(Camera &camera) {
    float speed = 20.0f * Engine::GetInstance().GetRenderer().GetDeltaTime();
    if (io::KeyPress(Key::w)) {
        camera.Translate(camera.Front() * speed);
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
        Engine::GetInstance().GetRenderer().Close();
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

    Engine& engine = Engine::GetInstance();

    engine.EnableUniformBuffer<LightInformation>();
    engine.EnableUniformBuffer<GlobalTransform>();

    Scene& scene = engine.CreateScene();
    engine.MakeCurrentScene(scene);

    // --1--
    GameObject& sphere = scene.CreateGameObject(); {
        sphere.CreateComponent<Mesh>(SimpleMesh::Sphere());
        auto& material = sphere.CreateComponent<Material>();
        material.SetShader(engine.GetDefaultShader());
        material.SetAlbedo(1, 1, 1);
        material.SetMetallic(0.1);
        material.SetRoughness(0.8);
        auto& transform = sphere.CreateComponent<Transform>();
        transform.SetPosition(0, 0, -10);
    }

    // --2--
    GameObject& ground = scene.CreateGameObject(); {
        ground.CreateComponent<Mesh>(SimpleMesh::Quad());
        auto& material = ground.CreateComponent<Material>();
        material.SetShader(engine.GetDefaultShader());
        material.SetAlbedo(1, 1, 1);
        material.SetMetallic(0.3);
        material.SetRoughness(0.5);
        auto& transform = ground.CreateComponent<Transform>();
        transform.SetPosition(0, -2, -10);
        transform.SetRotation(1, 0, 0, -90);
        transform.SetScale(10, 10, 10);
    }

    // --3--
    auto light_color = glm::vec3 {100, 100, 0 };
    auto light_position = glm::vec3 {0, 6, -6};
    auto light = PointLight(light_position, light_color);
    // Set direction
    light.direction = {0, 0, 10}; - light_position;
    scene.CreateLight(light);

    GameObject& lamp = scene.CreateGameObject(); {
        lamp.CreateComponent<Mesh>(SimpleMesh::Sphere());
        auto& material = lamp.CreateComponent<Material>();
        material.SetShader(engine.GetDefaultShader());
        material.SetEmissive(light_color);

        auto& transform = lamp.CreateComponent<Transform>();
        transform.SetPosition(light_position);
        transform.SetScale(0.33);
    }

    scene.CreateSkybox();

    scene.Build();

    Renderer& renderer = engine.GetRenderer();
    while (!renderer.ShouldEnd()) {
        renderer.UpdateBeforeRendering();
        processInput(scene.GetCurrentCamera());

        scene.Update();

        renderer.UpdateAfterRendering();
    }

    return 0;
}
