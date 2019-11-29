//
// Created by Krisu on 2019-11-19.
//

#ifndef RENDER_LAB_TESTPBRMETALBALL_HPP
#define RENDER_LAB_TESTPBRMETALBALL_HPP

#include <string>
#include <glad/glad.h>
#include <Renderer.hpp>
#include <Shader.hpp>
#include <Model.hpp>
#include <GLFW_Window.hpp>
#include <Camera.hpp>

class PbrApp : public Renderer {
public:
    explicit PbrApp(GLFW_Window &window);
    void render() override;

private:
    void processKeyboard() override;

private:
    Shader shader;
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    GLFW_Window &window;
    Camera camera;

    unsigned int albedo, normal, metallic, roughness, ao;

    std::vector<glm::vec3> lightPositions, lightColors;

    int nrRows = 7;
    int nrColumns = 7;
    float spacing = 2.5;
};

PbrApp::PbrApp(GLFW_Window &window) : window(window) {
    shader = Shader("./shaders/pbr.vert", "./shaders/pbr.frag");
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));

    glEnable(GL_DEPTH_TEST);

    std::string file_prefix = "./resources/pbr/";
    albedo = loadTexture(file_prefix + "rustediron2_basecolor.png");
    normal = loadTexture(file_prefix + "rustediron2_normal.png");
    metallic = loadTexture(file_prefix + "rustediron2_metallic.png");
    roughness = loadTexture(file_prefix + "rustediron2_roughness.png");
    ao = loadTexture(file_prefix + "ao.png");

    lightPositions = {
            {-10.0f,  10.0f, 10.0f},
            { 10.0f,  10.0f, 10.0f},
            {-10.0f, -10.0f, 10.0f},
            { 10.0f, -10.0f, 10.0f}
    };
    lightColors = {
            {300.0f, 300.0f, 300.0f},
            {300.0f, 300.0f, 300.0f},
            {300.0f, 300.0f, 300.0f},
            {300.0f, 300.0f, 300.0f}
    };

    nrRows = 7;
    nrColumns = 7;
    spacing = 2.5;

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
            (float)window.width() / window.height(), 0.1f, 100.0f);
    shader.use();
    shader.setMat4("projection", projection);

    shader.setInt("albedo", 0);
    shader.setInt("normalMap", 1);
    shader.setInt("metallicMap", 2);
    shader.setInt("roughnessMap", 3);
    shader.setInt("aoMap", 4);

}

void PbrApp::render() {
    processKeyboard();

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    glm::mat4 view = camera.GetViewMatrix();
    shader.setMat4("view", view);
    shader.setVec3("camPos", camera.Position);

    glm::mat4 model;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, albedo);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, metallic);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, roughness);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, ao);

    // render lights and set lights uniforms
    for (int i = 0; i < lightPositions.size(); i++) {
        glm::vec3 newPos = lightPositions[i] +
                           glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
        shader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
        shader.setVec3("lightColors[" + std::to_string(i) + "]", lightColors[i]);

        model = glm::translate(model, newPos);
        model = glm::scale(model, glm::vec3(0.5f));
        shader.setMat4("model", model);
        Primitive::renderSphere();
    }

    shader.use();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0, 0, -10));
    model = glm::rotate(model, window.getTime(),  glm::vec3(0, 1, 0));

    shader.setMat4("model", model);
    Primitive::renderSphere();

    window.swapBuffers();
    window.pollEvents();
}

void PbrApp::processKeyboard() {
    // Keyboard
    if (window.keyPress(GLFW_KEY_ESCAPE)) {
        window.close();
    } else if (window.keyPress(GLFW_KEY_W)) {
        camera.processKeyboard(FORWARD, deltaTime);
    } else if (window.keyPress(GLFW_KEY_S)) {
        camera.processKeyboard(BACKWARD, deltaTime);
    } else if (window.keyPress(GLFW_KEY_A)) {
        camera.processKeyboard(LEFT, deltaTime);
    } else if (window.keyPress(GLFW_KEY_D)) {
        camera.processKeyboard(RIGHT, deltaTime);
    }
}


#endif //RENDER_LAB_TESTPBRMETALBALL_HPP
