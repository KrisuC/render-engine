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
#include "Light.hpp"

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
    static_assert(std::is_base_of_v<Component, DirectionalLight>, "fuck");

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
    GameObject& lamp = scene.CreateGameObject(); {
        glm::vec3 light_color = glm::vec3 { 1, 1, 1 } * 2.0f;
        glm::vec3 light_position = glm::vec3{-1, 3, 1} * 5.0f;

        lamp.CreateComponent<Mesh>(SimpleMesh::Sphere());
        auto& material = lamp.CreateComponent<Material>();
        material.SetShader(engine.GetDefaultShader());
        material.SetEmissive(light_color);

        auto& transform = lamp.CreateComponent<Transform>();
        transform.SetPosition(glm::vec3{-1, 3, 1} * 5.0f);
        transform.SetScale(0.33);

        auto& light = lamp.CreateComponent<DirectionalLight>();
        light.SetColor(light_color);
        // TODO: fix with Transform
        light.position = light_position;
        light.direction = glm::vec3{0, 0, -10} - light.position;
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
