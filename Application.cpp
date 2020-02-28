//
// Created by Krisu on 2019-11-22.
//

#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Application.hpp"
#include "engine/Model.hpp"
#include "engine/Skybox.hpp"
#include "engine/Material.hpp"
#include "engine/SceneGraph.hpp"
#include "Application.hpp"

std::string Application::glsl_version;
GLFWwindow *Application::window = nullptr;

static void glfwErrorCallback(int error, const char *description) {
    std::cerr << "Glfw Error " << error << ": " << description << std::endl;
}

// TODO: put these in Renderer
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
    window = glfwCreateWindow(width, height, "Render core", nullptr, nullptr);
    if (!window) {
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    // vsync
    glfwSwapInterval(1);
    // MSAA
//    glfwWindowHint(GLFW_SAMPLES, 4);
//    glEnable(GL_MULTISAMPLE);

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
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));
}



void Application::renderScene() {
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int w, h;
    glfwGetWindowSize(window, &w, &h);
    auto projection = glm::perspective(glm::radians(camera.Zoom), (float)w/h, 0.1f, 100.0f);
    auto view       = camera.GetViewMatrix();

//    sceneGraph.root()->setGlobalShaderValue("projection", projection);
//    sceneGraph.root()->setGlobalShaderValue("view", view);
//    sceneGraph.root()->setGlobalShaderValue("camPos", camera.Position);
//    sceneGraph.renderScene();
    // -------------------Preparing----------------------------
    static std::vector<PointLight> lights = {
            {{-10.0f, 10.0f,  0.0f}, {300.0f, 300.0f, 300.0f}},
            {{10.0f,  10.0f,  0.0f}, {300.0f, 300.0f, 300.0f}},
            {{-10.0f, -10.0f, 0.0f}, {300.0f, 300.0f, 300.0f}},
            {{10.0f,  -10.0f, 0.0f}, {300.0f, 300.0f, 300.0f}},
    };
    static Mesh sphere_mesh = SimpleMesh::sphere();
    static Mesh cube_mesh = SimpleMesh::cube();
    static std::vector<Material *> materials;
    static bool first_run = true;
    if (first_run) {
        auto *irradiance_map = new Texture{skybox.getIrradianceMap(),
                                           TextureType::CubeMap};
        auto *prefilter_map = new Texture{skybox.getPrefilterMap(),
                                          TextureType::CubeMap};
        auto *brdfLUT_map = new Texture{skybox.getBrdfLUTTexture(),
                                        TextureType::Texture2D};

        for (int i = 0; i < 3; i++) {
            materials.push_back(new Material);
        }
        materials.push_back(new Material{"asset/metal03/", ".jpg"});
        materials.push_back(new Material{"asset/ground36/", ".jpg"});
        materials.push_back(new Material{"asset/marble05/", ".jpg"});

        for (auto material : materials) {
            material->appendTexture("irradiance_map", irradiance_map);
            material->appendTexture("prefilter_map", prefilter_map);
            material->appendTexture("brdfLUT_map", brdfLUT_map);
        }
        first_run = false;
    }
    // Global Settings update for each pass
    {
        for (auto material : materials) {
            material->shader->use();
            material->shader->set("projection", projection);
            material->shader->set("view", view);
            material->shader->set("cam_pos", camera.Position);
            for (int i = 0; i < 4; i++) {
                auto str_i = std::to_string(i);
                material->shader->use();
                material->shader->set("lightPositions["+str_i+"]",
                                      lights[i].position + glm::vec3(sin(glfwGetTime()*5)));
                material->shader->set("lightColors["+str_i+"]", lights[i].color);
            }
        }
        for (int i = 0; i < materials.size(); i++) {
            materials[i]->shader->use();
            materials[i]->shader->set("model",
                    glm::translate(glm::mat4(1.f), {3.f*(i-materials.size()/2.f), 0, -3}));
        }
    }
    // First
    materials[0]->shader->use();
    materials[0]->shader->set("use_texture", 0);
    // setting material
    materials[0]->setAlbedo(ui.albedo);
    materials[0]->setRoughness(ui.roughness);
    materials[0]->setMetallic(ui.metallic);
    materials[0]->updateShader(<#initializer#>);
    sphere_mesh.render();

    // Second:
    materials[1]->shader->use();
    materials[1]->shader->set("use_texture", 0);
    materials[1]->setAlbedo({0.4, 0.4, 0.4});
    materials[1]->setRoughness(0.1);
    materials[1]->setMetallic(0.1);
    materials[1]->updateShader(<#initializer#>);
    sphere_mesh.render();

    // Third
    materials[2]->shader->use();
    static auto *albedo = new Texture {"asset/rust-steel/albedo.png"};
    static auto *ao = new Texture {"asset/rust-steel/ao.png"};
    static auto *metallic = new Texture {"asset/rust-steel/metallic.png"};
    static auto *normal = new Texture {"asset/rust-steel/normal.png"};
    static auto *roughness = new Texture {"asset/rust-steel/roughness.png"};
    materials[2]->setAlbedo(albedo);
    materials[2]->setAO(ao);
    materials[2]->setMetallic(metallic);
    materials[2]->setNormal(normal);
    materials[2]->setRoughness(roughness);
    materials[2]->shader->set("use_texture", 1);
    materials[2]->updateShader(<#initializer#>);
    sphere_mesh.render();

    // Fourth
    materials[3]->shader->use();
    materials[3]->shader->set("use_texture", 1);
    materials[3]->updateShader(<#initializer#>);
    sphere_mesh.render();

    // Fiveth
    materials[4]->shader->use();
    materials[4]->shader->set("use_texture", 1);
    materials[4]->updateShader(<#initializer#>);
    sphere_mesh.render();

    // Sixth
    materials[5]->shader->use();
    materials[5]->shader->set("use_texture", 1);
    materials[5]->updateShader(<#initializer#>);
    sphere_mesh.render();

    // Sky box
    skybox.shader.useShaderProgram();
    skybox.shader.set("view", view);
    skybox.shader.set("projection", projection);
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

