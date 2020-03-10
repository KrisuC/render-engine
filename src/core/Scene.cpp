//
// Created by Krisu on 2020-02-05.
//

#include "Scene.hpp"

#include <glm/gtc/type_ptr.hpp>

void Scene::Build() {
    IBL const& ibl = up_skybox->GetIBL();
    for (auto& up_gameObject : up_gameObjects) {
        /* Check if it's renderable */
        try {
            auto& material = up_gameObject->GetComponent<Material>();
            auto& shader = material.GetShader();

            material.setIBLTextures(ibl);
            material.updateShaderUniform();

            shader.UseShaderProgram();
        } catch (NoComponent&) {
            // TODO: Change with better mechanism
            continue;
        }

        /* Call BeforeRenderLoop() for all components */
        for (auto it : up_gameObject->componentsMap) {
            auto & component = it.second;
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
    Engine& engine = Engine::GetInstance();
    // GLobalTransform Uniform Block
    UniformBlock& proj_view_matrices =
            engine.GetUniformBlock("GlobalTransform");
    auto [w, h] = engine.GetRenderer().GetWindowSize();

    glm::mat4 projection = glm::perspective(glm::radians(camera.GetFovy()),
                                            static_cast<float>(w)/h, 0.1f, 1000.0f);
    glm::mat4 view = camera.GetViewMatrix();
    proj_view_matrices.SetBufferSubData(0, sizeof(glm::mat4), glm::value_ptr(projection));
    proj_view_matrices.SetBufferSubData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));

    // LightInformation Uniform Block
    UniformBlock& lightInfo = engine.GetUniformBlock("LightInformation");
    const int N = sizeof(float);
    using glm::value_ptr;
    for (int i = 0; i < lights.size(); i++) {
        const int elem_offset = 240 * N * i; // element of struct's size
        lightInfo.SetBufferSubData(elem_offset + 0, 3 * N, value_ptr(lights[i].position));
        lightInfo.SetBufferSubData(elem_offset + 3 * N,  N, &lights[i].cone_angle_in_radian);

        lightInfo.SetBufferSubData(elem_offset + 4 * N, 3 * N, value_ptr(lights[i].direction));
        lightInfo.SetBufferSubData(elem_offset + 7 * N, N, &lights[i].ltype);

        lightInfo.SetBufferSubData(elem_offset + 8 * N, 3 * N, value_ptr(lights[i].color));
    }
    int lights_size = lights.size();
    glm::vec3 camera_pos = camera.Position();
    lightInfo.SetBufferSubData(240*N, 3*N, value_ptr(camera_pos));
    lightInfo.SetBufferSubData(243*N, N, &lights_size);


    auto& renderer = engine.GetRenderer();
    for (auto& up_gameObject : up_gameObjects) {
        // BEFORE
        for (auto it : up_gameObject->componentsMap) {
            auto & component = it.second;
            component->BeforeRenderPass();
        }
        // RENDERING
        renderer.Render(*up_gameObject);
        up_skybox->Render();
        // AFTER
        for (auto it : up_gameObject->componentsMap) {
            auto & component = it.second;
            component->AfterRenderPass();
        }
    }
}
