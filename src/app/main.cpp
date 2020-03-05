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
    engine.MakeCurrentScene(scene);

    scene.CreateLight(PointLight({2, 2, 2},
                                 {1, 1, 1}));

    GameObject& metalSphere = scene.CreateGameObject();
    metalSphere.CreateComponent<Mesh>(SimpleMesh::Sphere());
    auto& m_sphere = metalSphere.CreateComponent<Material>(); {
        m_sphere.SetShader(&Shader::DefaultShader());
        m_sphere.SetAlbedo(1, 1, 1);
        m_sphere.SetMetallic(0.9);
        m_sphere.SetRoughness(0.02);
    }
    auto& tr_sphere = metalSphere.CreateComponent<Transform>(); {
        tr_sphere.SetPosition(0, 0, -10);
    }

    GameObject& ground = scene.CreateGameObject();
    ground.CreateComponent<Mesh>(SimpleMesh::Quad());
    auto& m_ground = ground.CreateComponent<Material>(); {
        m_ground.SetShader(&Shader::TestShader());
        m_ground.SetAlbedo(0.5, 0.5, 0.5);
        m_ground.SetMetallic(0.1);
        m_ground.SetRoughness(0.9);
    }
    auto& tr_ground = ground.CreateComponent<Transform>(); {
        tr_ground.SetPosition(0, -2, -10);
        tr_ground.SetRotation(1, 0, 0, 90);
        tr_ground.SetScale(10, 10, 10);
    }

    scene.CreateSkybox();

    scene.Build();

    Renderer& renderer = engine.GetRenderer();
    while (!renderer.ShouldEnd()) {
        renderer.UpdateBeforeRendering();
        processInput(scene.GetCurrentCamera());

//        tr_sphere.SetRotation(0, 1, 0, glfwGetTime() * 10);
        scene.Update();

        renderer.UpdateAfterRendering();
    }


    /* Game code ends here */

    return 0;
}


