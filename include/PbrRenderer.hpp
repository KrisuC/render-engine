//
// Created by Krisu on 2019-11-19.
//

#ifndef RENDER_ENGINE_TESTPBRMETALBALL_HPP
#define RENDER_ENGINE_TESTPBRMETALBALL_HPP

#include <string>

#include <Renderer.hpp>
#include <Shader.hpp>
#include <Model.hpp>
#include <Camera.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class PbrRenderer : public Renderer {
public:
    PbrRenderer() = default;
    void prepareScene() override;
    void renderScene() override;

private:
    Shader shader;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    Camera camera;

    Texture albedo, normal, metallic, roughness, ao;

    std::vector<glm::vec3> lightPositions, lightColors;

    int nrRows = 7;
    int nrColumns = 7;
    float spacing = 2.5;
};

#endif //RENDER_ENGINE_TESTPBRMETALBALL_HPP
