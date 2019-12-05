//
// Created by Krisu on 2019-10-27.
//

#ifndef RENDER_ENGINE_APPLICATION_HPP
#define RENDER_ENGINE_APPLICATION_HPP

#define scene

// when using glfw3, glad must be included
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader.hpp>
#include <Camera.hpp>
#include <Texture.hpp>
#include <glm/glm.hpp>

// The base class of App
class Application {
public:
    Application(int argc, char *argv[]);
    inline int exec() {
        initializeContext();
        initializeScene();
        while (!applicationEnds) {
            renderPass();
        }
        cleanUp();
        return 0;
    }

private:
    void initializeContext();
    void initializeScene();
    void processKeyboard();
    void renderPass();
    void renderScene();
    void cleanUp();

private:
    bool applicationEnds = false;
    GLFWwindow *window = nullptr;

private scene:
    // Scnene configuration
    Shader pbrShader;
    Shader pbrNTShader;
    Shader equirectToCubemapShader;
    Shader skyboxShader;
    Shader irradianceShader;
    Shader prefilterShader;
    Camera camera;

    Texture albedo,
            normal,
            metallic,
            roughness,
            ao;

    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;

    unsigned int envCubemap;
    unsigned int irradianceMap;
    unsigned int prefilterMap;
};

#endif //RENDER_ENGINE_APPLICATION_HPP
