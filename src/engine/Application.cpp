//
// Created by Krisu on 2019-11-22.
//

#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "engine/Application.hpp"
#include "Model.hpp"
#include "objects/Skybox.hpp"
#include "basic/Material.hpp"
#include "engine/SceneGraph.hpp"
#include "Application.hpp"

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
    // TODO: replace all these shits with a scene description files
    //       and some function for generating scene graph
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));
    skybox.prepare();

    // TODO: auto scene graph construction from scene description file
    // Scene graph generation (by hand) for testing


    // TODO: Scene Graph!!!!! (After I fix Object)
//    sceneGraph.setRoot(new SGNode());
//    // setting up lights
//    sceneGraph.root()->setLights(lights);
//    // part 1
//    auto frontSphere = new CookTorrancePBR();
//    sceneGraph.root()->appendObjects(frontSphere);
//    sceneGraph.root()->setLocalTransform(
//            Transform(glm::translate(glm::mat4(1.f), {0, 0, -5})));
//
//    // part 2
//    auto backSpheresGroup = new SGNode();
//    sceneGraph.root()->appendNodes(backSpheresGroup);
//    backSpheresGroup->setLocalTransform(
//            Transform(glm::translate(glm::mat4(1.f), {0, 0, -5})));
//    int rows = 7, cols = 7;
//    float spacing = 2.5;
//    backSpheresGroup->setGlobalShaderValue("albedoVal", 0.5, 0.0, 0.0);
//    backSpheresGroup->setGlobalShaderValue("aoVal", 1.0f);
//    for (int row = 0; row < rows; row++) {
//        for (int col = 0; col < cols; col++) {
//            glm::vec3 pos = {
//                    (float(col) - cols / 2.f) * spacing,
//                    (float(row) - rows / 2.f) * spacing,
//                    -10.f
//            };
//            auto lt = glm::translate(glm::mat4(1.f), pos);
//            auto backSphere = new CookTorrancePBR();
//            backSphere->setLocalTransform(Transform(lt));
//            backSphere->setShaderUnif("metallicVal", (float)row / rows);
//            backSphere->setShaderUnif("roughnessVal",
//                    glm::clamp((float)col / cols, 0.05f, 1.0f));
//            backSpheresGroup->appendObjects(backSphere);
//        }
//    }
//    sceneGraph.root()->updateLights();
//    sceneGraph.prepareScene();
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
    static Mesh sphere_mesh = Primitive::sphere();
    static Mesh cube_mesh = Primitive::cube();
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
        materials.push_back(new Material{"resources/metal03/", ".jpg"});
        materials.push_back(new Material{"resources/ground36/", ".jpg"});
        materials.push_back(new Material{"resources/marble05/", ".jpg"});

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
    materials[0]->updateShader();
    sphere_mesh.render();

    // Second:
    materials[1]->shader->use();
    materials[1]->shader->set("use_texture", 0);
    materials[1]->setAlbedo({0.4, 0.4, 0.4});
    materials[1]->setRoughness(0.1);
    materials[1]->setMetallic(0.1);
    materials[1]->updateShader();
    sphere_mesh.render();

    // Third
    materials[2]->shader->use();
    static auto *albedo = new Texture {"resources/rust-steel/albedo.png"};
    static auto *ao = new Texture {"resources/rust-steel/ao.png"};
    static auto *metallic = new Texture {"resources/rust-steel/metallic.png"};
    static auto *normal = new Texture {"resources/rust-steel/normal.png"};
    static auto *roughness = new Texture {"resources/rust-steel/roughness.png"};
    materials[2]->setAlbedo(albedo);
    materials[2]->setAO(ao);
    materials[2]->setMetallic(metallic);
    materials[2]->setNormal(normal);
    materials[2]->setRoughness(roughness);
    materials[2]->shader->set("use_texture", 1);
    materials[2]->updateShader();
    sphere_mesh.render();

    // Fourth
    materials[3]->shader->use();
    materials[3]->shader->set("use_texture", 1);
    materials[3]->updateShader();
    sphere_mesh.render();

    // Fiveth
    materials[4]->shader->use();
    materials[4]->shader->set("use_texture", 1);
    materials[4]->updateShader();
    sphere_mesh.render();

    // Sixth
    materials[5]->shader->use();
    materials[5]->shader->set("use_texture", 1);
    materials[5]->updateShader();
    sphere_mesh.render();

    // Sky box
    skybox.shader.use();
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

