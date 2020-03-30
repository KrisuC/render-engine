#include <fstream>
#include <sstream>
#include <iostream>

#include "Scene.hpp"
#include "WindowManager.hpp"
#include "Engine.hpp"
#include "StaticMesh.hpp"
#include "Transform.hpp"
#include "PBR_Material.hpp"
#include "Debug.hpp"
#include "IO.hpp"
#include "LightInformation.hpp"
#include "GlobalTransform.hpp"
#include "Light.hpp"
#include "ModelManager.hpp"
#include "MeshRenderer.hpp"

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
    Engine &engine = Engine::GetInstance();

    engine.EnableUniformBuffer<LightInformation>();
    engine.EnableUniformBuffer<GlobalTransform>();

    Scene &scene = engine.CreateScene();
    engine.MakeCurrentScene(scene);
    scene.CreateSkybox();
    const IBL& ibl = scene.GetSkybox().GetIBL();

    // --1--
    GameObject &sphere = scene.CreateGameObject();
    PBR_Material m_sphere;
    m_sphere.SetShader(engine.CreateStandardShader());
    m_sphere.SetAlbedo(1, 1, 1);
    m_sphere.SetMetallic(0.1);
    m_sphere.SetRoughness(0.8);
    m_sphere.SetIBLTextures(ibl);
    auto &mr_sphere = sphere.CreateComponent<MeshRenderer>(
            BuiltinMesh::GetSphere(), m_sphere);
    auto &t_sphere = sphere.CreateComponent<Transform>();
    t_sphere.SetPosition(0, 0, -10);


    // --2--
    GameObject &ground = scene.CreateGameObject();
    PBR_Material m_ground;
    m_ground.SetShader(engine.CreateStandardShader());
    m_ground.SetAlbedo(1, 1, 1);
    m_ground.SetMetallic(0.3);
    m_ground.SetRoughness(0.5);
    m_ground.SetIBLTextures(ibl);
    auto &mr_ground = ground.CreateComponent<MeshRenderer>(BuiltinMesh::GetQuad(), m_ground);
    auto &t_ground = ground.CreateComponent<Transform>();
    t_ground.SetPosition(0, -2, -10);
    t_ground.SetRotation(1, 0, 0, -90);
    t_ground.SetScale(10, 10, 10);

    // --3--
    GameObject& lamp = scene.CreateGameObject();
    glm::vec3 light_color = glm::vec3{1, 1, 1} * 2.0f;
    glm::vec3 light_position = glm::vec3{-1, 3, 1} * 5.0f;

    PBR_Material m_lamp;
    m_lamp.SetShader(engine.CreateStandardShader());
    m_lamp.SetEmissive(light_color);
    m_lamp.SetIBLTextures(ibl);

    auto &mr_lamp = lamp.CreateComponent<MeshRenderer>(BuiltinMesh::GetSphere(), m_lamp);
    auto &t_lamp = lamp.CreateComponent<Transform>();
    t_lamp.SetPosition(glm::vec3{-1, 3, 1} * 5.0f);
    t_lamp.SetScale(0.33);

    auto &light = lamp.CreateComponent<DirectionalLight>();
    light.SetColor(light_color);
    // TODO: fix with Transform
    light.position = light_position;
    light.direction = glm::vec3{0, 0, -10} - light.position;


//    ModelManager sponza;
//    sponza.LoadModel("asset/sponza/sponza.obj");

    WindowManager& window = engine.GetRenderer();
    while (!window.ShouldEnd()) {
        window.UpdateBeforeRendering();
        processInput(scene.GetCurrentCamera());

        scene.Update();

        window.UpdateAfterRendering();
    }

    return 0;
}
