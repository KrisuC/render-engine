//
// Created by Krisu on 2020-02-05.
//


#include "Scene.hpp"
#include "GlobalTransform.hpp"
#include "LightInformation.hpp"
#include "Engine.hpp"
#include "GameObject.hpp"
#include "Skybox.hpp"
#include "Light.hpp"
#include "PBR_Material.hpp"
#include "WindowManager.hpp"
#include "StaticMesh.hpp"
#include "DirectionalShadow.hpp"
#include "Transform.hpp"

GameObject &Scene::CreateGameObject() {
    upGameObjects.emplace_back(std::make_unique<GameObject>());
    return *upGameObjects.back();
}

void Scene::Update() {
    Engine& engine = Engine::GetInstance();
    auto& renderer = engine.GetRenderer();

    /* run Start for all components only once */
    static bool first_frame = true;
    if (first_frame) {
        /* Call Start() for all components */
        for (auto& pGameObject : upGameObjects) {
            for (auto &up_component : pGameObject->components_map) {
                auto & component = up_component.second;
                component->Start();
            }
        }
        first_frame = false;
    }

    /* Scene update */
    renderer.ResetViewport();
    for (auto& pGameObject : upGameObjects) {
        for (auto &it : pGameObject->components_map) {
            auto & component = it.second;
            component->Update();
        }
        for (auto &it : pGameObject->components_map) {
            auto & component = it.second;
            component->LateUpdate();
        }
    }
    upSkybox->Render();
}

void Scene::RenderMeshes(Shader &shader) {
    shader.UseShaderProgram();
    for (auto& pGameObject : upGameObjects) {
        try {
            auto& mesh = pGameObject->GetComponent<MeshRenderer>().mesh;
            auto& transform = pGameObject->GetComponent<Transform>();
            shader.Set("model", transform.GetMatrix());
            mesh.DrawCall();
        } catch (NoComponent &) {
            continue;
        }
    }
}

