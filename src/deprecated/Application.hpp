//
// Created by Krisu on 2019-10-27.
//

#ifndef RENDER_ENGINE_APPLICATION_HPP
#define RENDER_ENGINE_APPLICATION_HPP

// when using glfw3, glad must be included
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "engine/Shader.hpp"
#include "engine/Camera.hpp"
#include "engine/Texture.hpp"
#include "engine/Light.hpp"
#include "engine/SceneGraph.hpp"
#include "engine/Skybox.hpp"

// TODO: remove Application
// The base class of App
class Application {
public:
    static Application &instance() {
        static Application app;
        return app;
    }
    void processArgs(int argc, char *argv[]);
    static void initializeContext();

    inline int exec() {
        prepareUI();
        prepareScene();
        while (!applicationEnds) {
            renderPass(); // call renderScene() inside
        }
        cleanUp();
        return 0;
    }

private:
    Application() = default;

    void prepareScene();
    void processKeyboard();
    void renderPass();
    void renderScene();
    void cleanUp();
    void prepareUI();

private:
    bool applicationEnds = false;
    static GLFWwindow *window;
    static std::string glsl_version;

private:
    // TODO: manage scene with scene graph. Obejects should be allocated
    //       in prepareScene() rather than insert here in application
    // Scnene configuration
    Camera camera;
    Skybox skybox {"asset/skybox/newport_loft.hdr"};
    SceneGraph sceneGraph {};

    // GUI controls variables
    struct {
        glm::vec3 albedo = {1, 1, 1};
        float     roughness = 0.1;
        float     metallic = 0.9;
        bool      useTexture = false;
        bool      useModel = false;
    } ui;
};

#endif //RENDER_ENGINE_APPLICATION_HPP
