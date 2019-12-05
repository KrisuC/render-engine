//
// Created by Krisu on 2019-11-22.
//

#include <Application.hpp>
#include <iostream>
#include <Model.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/matrix.hpp>

static void glfwErrorCallback(int error, const char *description) {
    std::cerr << "Glfw Error " << error << ": " << description << std::endl;
}

Application::Application(int argc, char **argv) {
    // TODO:
    // deal with arguments, if there is any
}

void Application::initializeContext(){
    // Setup ImGui context
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        exit(-1);
    }

    const char *glsl_version = "#version 410";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}



void Application::renderPass() {
    applicationEnds = glfwWindowShouldClose(window);

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static bool show_another_window = false;
    static ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
    // A simple window
    {
        static float f = 0.0f;
        static int cnt = 0;

        ImGui::Begin("Hello world!");

        ImGui::Text("This is a label");
        ImGui::Checkbox("Another window", &show_another_window);

        ImGui::SliderFloat("f", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float *)&clear_color);

        if (ImGui::Button("cnt plus plus")) {
            cnt++;
        }
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



void Application::initializeScene() {
    pbrShader = Shader("./shaders/pbr.vert", "./shaders/pbr.frag");
    pbrNTShader = Shader("./shaders/pbr.vert",
                         "./shaders/pbr_notexture.frag");
    equirectToCubemapShader = Shader(
            "./shaders/cubemap.vert",
            "./shaders/equirectangular_to_cubemap.frag");
    skyboxShader = Shader("./shaders/skybox.vert", "./shaders/skybox.frag");
    irradianceShader = Shader("./shaders/cubemap.vert",
                              "./shaders/irradiance_convolution.frag");
    prefilterShader = Shader("./shaders/cubemap.vert",
                             "./shaders/prefilter_map.frag");

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));

    albedo=Texture("./resources/pbr/rustediron2_basecolor.png"),
    normal=Texture("./resources/pbr/rustediron2_normal.png"),
    metallic=Texture("./resources/pbr/rustediron2_metallic.png"),
    roughness=Texture("./resources/pbr/rustediron2_roughness.png"),
    ao=Texture("./resources/pbr/ao.png");

    lightPositions = {
            {-10.0f,  10.0f, 0.0f},
            { 10.0f,  10.0f, 0.0f},
            {-10.0f, -10.0f, 0.0f},
            { 10.0f, -10.0f, 0.0f}
    };
    lightColors = {
            {300.0f, 300.0f, 300.0f},
            {300.0f, 300.0f, 300.0f},
            {300.0f, 300.0f, 300.0f},
            {300.0f, 300.0f, 300.0f}
    };

    // convert equirectangular to cubemap
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    unsigned int dim = 4096;

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, dim, dim);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER, captureRBO);

    Texture hdrTexture("./resources/ibl/newport_loft.hdr", "hdr");

    // generate corresponding cubemap
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                     dim, dim, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glm::mat4 captureProjection
        = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 10.f);
    glm::vec3 orig = {0, 0, 0};
    glm::mat4 captureViews[] = {
            // eye, center, up
        glm::lookAt(orig, glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(orig, glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(orig, glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
        glm::lookAt(orig, glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
        glm::lookAt(orig, glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
        glm::lookAt(orig, glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
    equirectToCubemapShader.use();
    equirectToCubemapShader.setValue("equirectangularMap", 0);
    equirectToCubemapShader.setValue("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture.id());

    glViewport(0, 0, dim, dim);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (int i = 0; i < 6; i++) {
        equirectToCubemapShader.setValue("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Primitive::renderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // generate irradiance map
    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    for (int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32,
                     0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // linear filter will handle the low resolution texture
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

    irradianceShader.use();
    irradianceShader.setValue("environmentMap", 0);
    irradianceShader.setValue("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

    glViewport(0, 0, 32, 32);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (int i = 0; i < 6; i++) {
        irradianceShader.setValue("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Primitive::renderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // generate prefilter map for ibl specular lighting
    int res_prfmap = 128;
    glGenTextures(1, &prefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    for (int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                res_prfmap, res_prfmap, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    prefilterShader.use();
    prefilterShader.setValue("environmentMap", 0);
    prefilterShader.setValue("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    unsigned int maxMipLevels = 5;
    for (int mip = 0; mip < maxMipLevels; mip++) {
        // resize framebuffer according to mip-level
        unsigned int mipWidth  = 128 * std::pow(0.5, mip);
        unsigned int mipHeight = 128 * std::pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, captureFBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        prefilterShader.setValue("roughness", roughness);
        for (int i = 0; i < 6; i++) {
            prefilterShader.setValue("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterMap, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            Primitive::renderCube();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void Application::renderScene() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    static glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                            (float)width / height, 0.1f, 100.0f);

    glm::mat4 view = camera.GetViewMatrix();

    pbrShader.use();
    pbrShader.setValue("projection", projection);
    pbrShader.setValue("view", view);
    pbrShader.setValue("camPos", camera.Position);

    pbrShader.setValue("albedoMap", 0);
    pbrShader.setValue("normalMap", 1);
    pbrShader.setValue("metallicMap", 2);
    pbrShader.setValue("roughnessMap", 3);
    pbrShader.setValue("aoMap", 4);

    pbrShader.setValue("irradianceMap", irradianceMap);

    glActiveTexture(GL_TEXTURE0);   glBindTexture(GL_TEXTURE_2D, albedo.id());
    glActiveTexture(GL_TEXTURE1);   glBindTexture(GL_TEXTURE_2D, normal.id());
    glActiveTexture(GL_TEXTURE2);   glBindTexture(GL_TEXTURE_2D, metallic.id());
    glActiveTexture(GL_TEXTURE3);   glBindTexture(GL_TEXTURE_2D, roughness.id());
    glActiveTexture(GL_TEXTURE4);   glBindTexture(GL_TEXTURE_2D, ao.id());

    // render lights and set lights uniforms
    assert(lightPositions.size() == lightColors.size());
    for (int i = 0; i < lightPositions.size(); i++) {
        glm::vec3 newPos = lightPositions[i] +
                           glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
        pbrShader.use();
        pbrShader.setValue("lightPositions[" + std::to_string(i) + "]", newPos);
        pbrShader.setValue("lightColors[" + std::to_string(i) + "]",
                           lightColors[i]);

        pbrNTShader.use();
        pbrNTShader.setValue("lightPositions[" + std::to_string(i) + "]",
                           newPos);
        pbrNTShader.setValue("lightColors[" + std::to_string(i) + "]",
                           lightColors[i]);
    }

    pbrShader.use();
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0, 0, -5));
    model = glm::rotate(model, (float)glfwGetTime(),  glm::vec3(0, 1, 0));

    pbrShader.setValue("model", model);
    Primitive::renderSphere();

    pbrNTShader.use();
    pbrNTShader.setValue("projection", projection);
    pbrNTShader.setValue("view", view);
    pbrNTShader.setValue("camPos", camera.Position);
    pbrNTShader.setValue("irradianceMap", irradianceMap);

    int rows = 7, cols = 7;
    float spacing = 2.5;

    pbrNTShader.setValue("albedo", 0.5f, 0.0f, 0.0f);
    pbrNTShader.setValue("ao", 1.0f);
    for (int row = 0; row < rows; row++) {
        pbrNTShader.setValue("metallic", (float)row / rows);
        for (int col = 0; col < cols; col++) {
            pbrNTShader.setValue("roughness",
                    glm::clamp((float)col / cols, 0.05f, 1.0f));

            glm::vec3 pos = {
                    (float(col) - cols / 2.f) * spacing,
                    (float(row) - rows / 2.f) * spacing,
                    -10.f
            };
            model = glm::translate(glm::mat4(1.f), pos);
            pbrNTShader.setValue("model", model);
            Primitive::renderSphere();
        }
    }

    skyboxShader.use();
    skyboxShader.setValue("view", view);
    skyboxShader.setValue("projection", projection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    Primitive::renderCube();
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
    static float move_speed = 2.f;
    static float rotate_speed = 1000.f;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(FORWARD, deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(BACKWARD, deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboard(LEFT, deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboard(RIGHT, deltaTime);
    } else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.processMouseMovement(deltaTime * -1000, 0);
    } else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.processMouseMovement(deltaTime *  1000, 0);
    }
}