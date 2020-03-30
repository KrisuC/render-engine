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
        for (auto& up_gameObject : upGameObjects) {
            for (auto &up_component : up_gameObject->components_map) {
                auto & component = up_component.second;
                component->Start();
            }
        }
        first_frame = false;
    }

    /* 1 - Updating Shared GPU memory */
    this->UpdateUniformBlocks();

    /* 2 - Scene update */
    renderer.ResetViewport();
    for (auto& upGameObject : upGameObjects) {
        for (auto &it : upGameObject->components_map) {
            auto & component = it.second;
            component->Update();
        }
        for (auto &it : upGameObject->components_map) {
            auto & component = it.second;
            component->LateUpdate();
        }
    }
    upSkybox->Render();
}


void Scene::UpdateUniformBlocks() {
    Engine& engine = Engine::GetInstance();
    /* GLobalTransform Uniform Block */
    auto& globalTransform = engine.GetUniformBuffer<GlobalTransform>();
    globalTransform.UpdateView(GetCurrentCamera().GetViewMatrix());
    globalTransform.UpdateProjeciton(GetCurrentCamera().GetProjectionMatrix());

    /* LightInformation Uniform Block */
    auto& lightInfo = engine.GetUniformBuffer<LightInformation>();
    lightInfo.UpdateLightSize(LightManager::GetLightsCount());
    lightInfo.UpdateCameraPosition(GetCurrentCamera().Position());
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

