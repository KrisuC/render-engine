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
    float delta_time = Engine::GetInstance().GetRenderer().GetDeltaTime();
    float speed = 20.0f * delta_time;
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

//    // --1--
//    GameObject &sphere = scene.CreateGameObject();
//    PBR_Material m_sphere;
//    m_sphere.SetShader(engine.GetStandardShader());
//    m_sphere.SetAlbedo(new Texture("asset/texture/rust-steel/albedo.png"));
//    m_sphere.SetRoughness(new Texture("asset/texture/rust-steel/roughness.png"));
//    m_sphere.SetMetallic(new Texture("asset/texture/rust-steel/metallic.png"));
//    m_sphere.SetNormal(new Texture("asset/texture/rust-steel/normal.png"));
//    m_sphere.SetIBL(ibl);
//    auto &mr_sphere = sphere.CreateComponent<MeshRenderer>(
//            BuiltinMesh::GetSphere(), m_sphere);
//    auto &t_sphere = sphere.CreateComponent<Transform>();
//    t_sphere.SetPosition(0, 0, -10);
//
//
//    // --2--
//    GameObject &ground = scene.CreateGameObject();
//    PBR_Material m_ground;
//    m_ground.SetShader(engine.GetStandardShader());
//    m_ground.SetAlbedo(1, 1, 1);
//    m_ground.SetMetallic(0.3);
//    m_ground.SetRoughness(0.5);
//    m_ground.SetIBL(ibl);
//    auto &mr_ground = ground.CreateComponent<MeshRenderer>(BuiltinMesh::GetQuad(), m_ground);
//    auto &t_ground = ground.CreateComponent<Transform>();
//    t_ground.SetPosition(0, -2, -10);
//    t_ground.SetRotation(1, 0, 0, -90);
//    t_ground.SetScale(10, 10, 10);


    // --3--
    GameObject& lamp = scene.CreateGameObject();
    glm::vec3 light_color = glm::vec3 {94.5, 85.5, 64.3} * 0.03f;
    glm::vec3 light_position = glm::vec3 { -97.3522 ,  280.053 ,  6.66883 };
    glm::vec3 light_direction = glm::vec3 { 0.34409 ,  -0.930441 ,  -0.126023 };

    PBR_Material m_lamp;
    m_lamp.SetShader(engine.GetStandardShader());
    m_lamp.SetEmissive(light_color);
    m_lamp.SetIBL(ibl);

    auto &mr_lamp = lamp.CreateComponent<MeshRenderer>(BuiltinMesh::GetSphere(), m_lamp);
    auto &t_lamp = lamp.CreateComponent<Transform>();
    t_lamp.SetPosition(light_position);
    t_lamp.SetScale(1);

    auto &light = lamp.CreateComponent<DirectionalLight>();
    light.SetColor(light_color);
    // TODO: fix with Transform
    light.position = light_position;
    light.direction = light_direction;

    // --4--
    ModelManager sponza;
    sponza.LoadModel("asset/sponza/sponza.obj");
    for (auto & mesh : sponza.m_meshes) {
        auto& gameObject = scene.CreateGameObject();
        auto &mr = gameObject.CreateComponent<MeshRenderer>(
                *mesh,
                *sponza.m_materials[mesh->material_index]
        );
        auto& transfrom = gameObject.CreateComponent<Transform>();
        transfrom.SetScale(0.1);
        transfrom.SetPosition(0, -2, 0);
    }


    WindowManager& window = engine.GetRenderer();
    auto& camera = Engine::GetInstance().GetCurrentScene().GetCurrentCamera();
    while (!window.ShouldEnd()) {
        window.UpdateBeforeRendering();
        processInput(scene.GetCurrentCamera());

//        light.position = camera.Position();
//        light.direction = camera.Front();
//
//        DEBUG_LOG("position", light.position.x, ", ", light.position.y, ", ", light.position.z);
//        DEBUG_LOG("direction", light.direction.x, ", ", light.direction.y, ", ", light.direction.z);


        Camera& camera = engine.GetCurrentScene().GetCurrentCamera();
        /* GLobalTransform Uniform Block */
        auto& globalTransform = engine.GetUniformBuffer<GlobalTransform>();
        globalTransform.UpdateView(camera.GetViewMatrix());
        globalTransform.UpdateProjeciton(camera.GetProjectionMatrix());

        /* LightInformation Uniform Block */
        auto& lightInfo = engine.GetUniformBuffer<LightInformation>();
        lightInfo.UpdateLightSize(LightManager::GetLightsCount());
        lightInfo.UpdateCameraPosition(camera.Position());

        scene.Update();

        window.UpdateAfterRendering();
    }

    return 0;
}
