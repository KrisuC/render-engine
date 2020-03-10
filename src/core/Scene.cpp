//
// Created by Krisu on 2020-02-05.
//


#include "Scene.hpp"
#include "GlobalTransformation.hpp"
#include "LightInformation.hpp"
#include "Engine.hpp"
#include "GameObject.hpp"
#include "Skybox.hpp"
#include "Light.hpp"
#include "Material.hpp"
#include "Renderer.hpp"


void Scene::Build() {
    IBL const& ibl = up_skybox->GetIBL();
    for (auto& up_game_object : up_gameObjects) {
        /* Check if it's renderable */
        try {
            auto& material = up_game_object->GetComponent<Material>();
            auto& shader = material.GetShader();

            material.setIBLTextures(ibl);
            material.updateShaderUniform();

            shader.UseShaderProgram();
        } catch (NoComponent&) {
            // TODO: Change with better mechanism
            continue;
        }

        /* Call BeforeRenderLoop() for all components */
        for (auto &up_component : up_game_object->components_map) {
            auto & component = up_component.second;
            component->BeforeRenderLoop();
        }
    }
}

void Scene::CreateLight(const Light &light) {
    lights.push_back(light);
}

GameObject &Scene::CreateGameObject() {
    up_gameObjects.emplace_back(std::make_unique<GameObject>());
    return *up_gameObjects.back();
}

void Scene::Update() {
    // Updating Shared GPU memory
    Engine& engine = Engine::GetInstance();
    // GLobalTransform Uniform Block
    auto& globalTransform = engine.GetUniformBuffer<GlobalTransformation>();
    auto [w, h] = engine.GetRenderer().GetWindowSize();
    glm::mat4 projection = glm::perspective(glm::radians(camera.GetFovy()),
                                            static_cast<float>(w)/h, 0.1f, 1000.0f);
    glm::mat4 view = camera.GetViewMatrix();

    globalTransform.UpdateView(view);
    globalTransform.UpdateProjeciton(projection);

    // LightInformation Uniform Block
    auto& lightInfo = engine.GetUniformBuffer<LightInformation>();
    const int N = sizeof(float);
    using glm::value_ptr;
    for (int i = 0; i < lights.size(); i++) {
        const int elem_offset = 240 * N * i; // element of struct's size
        lightInfo.UpdateLight(i, lights[i]);
    }
    lightInfo.UpdateLightSize(lights.size());
    lightInfo.UpdateCameraPosition(camera.Position());

    // Scene update
    auto& renderer = engine.GetRenderer();
    for (auto& up_gameObject : up_gameObjects) {
        // BEFORE
        for (auto &it : up_gameObject->components_map) {
            auto & component = it.second;
            component->BeforeRenderPass();
        }
        // RENDERING
        renderer.Render(*up_gameObject);
        up_skybox->Render();
        // AFTER
        for (auto &it : up_gameObject->components_map) {
            auto & component = it.second;
            component->AfterRenderPass();
        }
    }
}


