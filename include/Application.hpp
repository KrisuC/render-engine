//
// Created by Krisu on 2019-10-27.
//

#ifndef RENDER_ENGINE_APPLICATION_HPP
#define RENDER_ENGINE_APPLICATION_HPP

// when using glfw3, glad must be included
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <PbrRenderer.hpp>
#include <Shader.hpp>
#include <Camera.hpp>
#include <Texture.hpp>
#include <glm/glm.hpp>

// The base class of App
class Application {
public:
    Application(int argc, char *argv[]);
    int exec();

private:
    void renderPass();
    void initialize();

    void renderScene();

    void cleanUp();

private:
    bool applicationEnds = false;
    GLFWwindow *window = nullptr;
    PbrRenderer pbrRenderer;

    struct Scene {
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
};

#endif //RENDER_ENGINE_APPLICATION_HPP
