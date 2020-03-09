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
    float speed = 20.0 * Engine::GetInstance().GetRenderer().GetDeltaTime();
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
    /* Game code begins here */
    Engine& engine = Engine::GetInstance();

    Scene& scene = engine.CreateScene();
    engine.MakeCurrentScene(scene);

    // --1--
    GameObject& sphere = scene.CreateGameObject();
    sphere.CreateComponent<Mesh>(SimpleMesh::Sphere());
    auto& m_sphere = sphere.CreateComponent<Material>(); {
        m_sphere.SetShader(engine.GetDefaultShader());
        m_sphere.SetAlbedo(1, 1, 1);
        m_sphere.SetMetallic(0.1);
        m_sphere.SetRoughness(0.8);
    }
    auto& tr_sphere = sphere.CreateComponent<Transform>(); {
        tr_sphere.SetPosition(0, 0, -10);
    }
    // --2--
    GameObject& ground = scene.CreateGameObject();
    ground.CreateComponent<Mesh>(SimpleMesh::Quad());
    auto& m_ground = ground.CreateComponent<Material>(); {
        m_ground.SetShader(engine.GetDefaultShader());
        m_ground.SetAlbedo(1, 1, 1);
        m_ground.SetMetallic(0.3);
        m_ground.SetRoughness(0.5);
    }
    auto& tr_ground = ground.CreateComponent<Transform>(); {
        tr_ground.SetPosition(0, -2, -10);
        tr_ground.SetRotation(1, 0, 0, -90);
        tr_ground.SetScale(10, 10, 10);
    }
    // --3--
    auto light_color = glm::vec3 {100};
    auto light_position = glm::vec3 {0, 6, -6};
    scene.CreateLight(PointLight(light_position, light_color));

    GameObject& lamp = scene.CreateGameObject();
    lamp.CreateComponent<Mesh>(SimpleMesh::Sphere());
    auto& m_lamp = lamp.CreateComponent<Material>(); {
        m_lamp.SetShader(engine.GetDefaultShader());
        m_lamp.SetEmissive(light_color);
    }
    auto& tr_lamp = lamp.CreateComponent<Transform>(); {
        tr_lamp.SetPosition(light_position);
        tr_lamp.SetScale(0.33);
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


    /* Game code ends here */

    return 0;
}


