//
// Created by Krisu on 2019-11-22.
//

#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "engine/Application.hpp"
#include "Model.hpp"
#include "objects/CookTorrancePbr.hpp"
#include "objects/Skybox.hpp"
#include "engine/SceneGraph.hpp"

std::string Application::glsl_version;
GLFWwindow *Application::window = nullptr;

static void glfwErrorCallback(int error, const char *description) {
    std::cerr << "Glfw Error " << error << ": " << description << std::endl;
}

void Application::initializeContext(){
    // Setup ImGui context
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        exit(-1);
    }

    glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    int width = 1280, height = 720;
    window = glfwCreateWindow(width, height, "Render engine", nullptr, nullptr);
    if (!window) {
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    if (gladLoadGL() == 0) {
        std::cerr << "Failed to initialize glad loader\n";
        exit(-1);
    }
}



void Application::prepareUI() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());
}


void Application::renderPass() {
    applicationEnds = glfwWindowShouldClose(window);

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static bool show_another_window = false;
    static ImVec4 ball_color = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    // A simple window
    {
        static float f = 0.0f;
        static int cnt = 0;

        ImGui::Begin("Settings");

        ImGui::Text("Properties: ");
        ImGui::Checkbox("Use Texture", &ui.useTexture);
        ImGui::Checkbox("Use Model", &ui.useModel);

        ImGui::SliderFloat("Roughness", &ui.roughness, 0.001f, 1.0f);
        ImGui::SliderFloat("Metallic", &ui.metallic, 0.001f, 1.0f);
        ImGui::ColorEdit3("Albedo", (float *)&ui.albedo.x);

        if (ImGui::Button("cnt plus plus")) {
            cnt++;
        }
        ImGui::Checkbox("Show another window", &show_another_window);
        ImGui::SameLine();
        ImGui::Text("cnt = %d", cnt);

        ImGui::Text("Frame rate: %.1f FPS", ImGui::GetIO().Framerate);

        ImGui::End();
    }
    // Another window
    if (show_another_window) {
        ImGui::Begin("Another window", &show_another_window);
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("close")) {
            show_another_window = false;
        }
        ImGui::End();
    }
    processKeyboard();

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Rendering scene here
    renderScene();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}



void Application::prepareScene() {
    // TODO: replace all these shits with a scene description files
    //       and some function for generating scene graph
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));
    skybox.prepare();

    // TODO: auto scene graph construction from scene description file
    // Scene graph generation (by hand) for testing
    std::vector<Light> lights = {
            {{-10.0f,  10.0f, 0.0f}, {300.0f, 300.0f, 300.0f}},
            {{ 10.0f,  10.0f, 0.0f}, {300.0f, 300.0f, 300.0f}},
            {{-10.0f, -10.0f, 0.0f}, {300.0f, 300.0f, 300.0f}},
            {{ 10.0f, -10.0f, 0.0f}, {300.0f, 300.0f, 300.0f}},
    };

    sceneGraph.setRoot(new SGNode());
    // setting up lights
    sceneGraph.root()->setLights(lights);
    // part 1
    auto frontSphere = new CookTorrancePbr();
    sceneGraph.root()->appendObjects(frontSphere);
    sceneGraph.root()->setLocalTransform(
            Transform(glm::translate(glm::mat4(1.f), {0, 0, -5})));
    // part 2
    auto backSpheresGroup = new SGNode();
    sceneGraph.root()->appendNodes(backSpheresGroup);
    backSpheresGroup->setLocalTransform(
            Transform(glm::translate(glm::mat4(1.f), {0, 0, -5})));
    int rows = 7, cols = 7;
    float spacing = 2.5;
    backSpheresGroup->setGlobalShaderValue("albedoVal", 0.5, 0.0, 0.0);
    backSpheresGroup->setGlobalShaderValue("aoVal", 1.0f);
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            glm::vec3 pos = {
                    (float(col) - cols / 2.f) * spacing,
                    (float(row) - rows / 2.f) * spacing,
                    -10.f
            };
            auto lt = glm::translate(glm::mat4(1.f), pos);
            auto backSphere = new CookTorrancePbr();
            backSphere->setLocalTransform(Transform(lt));
            backSphere->setShaderUnif("metallicVal", (float)row / rows);
            backSphere->setShaderUnif("roughnessVal",
                    glm::clamp((float)col / cols, 0.05f, 1.0f));
            backSpheresGroup->appendObjects(backSphere);
        }
    }
    sceneGraph.root()->updateLights();
    sceneGraph.prepareScene();
}



void Application::renderScene() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                            (float)width / height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    sceneGraph.root()->setGlobalShaderValue("projection", projection);
    sceneGraph.root()->setGlobalShaderValue("view", view);
    sceneGraph.root()->setGlobalShaderValue("camPos", camera.Position);
    sceneGraph.renderScene();

    skybox.setShaderUnif("view", view);
    skybox.setShaderUnif("projection", projection);
    skybox.render();
}



void Application::cleanUp() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}



void Application::processKeyboard() {
    float deltaTime = 1.0 / ImGui::GetIO().Framerate;

#define if_press(GLFW_KEY_XXX, function_call) \
    if (glfwGetKey(window, GLFW_KEY_XXX) == GLFW_PRESS) { function_call; }

    if_press(GLFW_KEY_ESCAPE, glfwSetWindowShouldClose(window, true));
    if_press(GLFW_KEY_W, camera.processKeyboard(FORWARD, deltaTime));
    if_press(GLFW_KEY_S, camera.processKeyboard(BACKWARD, deltaTime));
    if_press(GLFW_KEY_A, camera.processKeyboard(LEFT, deltaTime));
    if_press(GLFW_KEY_D, camera.processKeyboard(RIGHT, deltaTime));
    if_press(GLFW_KEY_Q, camera.processMouseMovement(deltaTime * -1000, 0));
    if_press(GLFW_KEY_E, camera.processMouseMovement(deltaTime *  1000, 0));
    if_press(GLFW_KEY_C, camera.processMouseMovement(0, deltaTime * -1000));
    if_press(GLFW_KEY_Z, camera.processMouseMovement(0, deltaTime *  1000));

#undef if_press
}

void Application::processArgs(int argc, char **argv) {
    // TODO: process programm arguments if there is any
}

