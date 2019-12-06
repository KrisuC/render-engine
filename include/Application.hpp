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
    Shader ctPbrShader;
    Shader equirectToCubemapShader;
    Shader skyboxShader;
    Shader irradianceShader;
    Shader prefilterShader;
    Shader brdfLUTShader;

    Camera camera;

    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;

    unsigned int envCubemap;
    unsigned int irradianceMap;
    unsigned int prefilterMap;
    unsigned int brdfLUTTexture;

    // GUI controls variables
    glm::vec3 ui_albedo = {0, 0, 0};
    float     ui_roughness = 0.001;
    float     ui_mettallic = 0.001;
    bool      ui_useTexture = false;
};

#endif //RENDER_ENGINE_APPLICATION_HPP
