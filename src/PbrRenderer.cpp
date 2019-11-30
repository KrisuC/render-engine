//
// Created by Krisu on 2019-11-30.
//

#include <PbrRenderer.hpp>
#include <imgui.h>

void PbrRenderer::prepareScene(){
    shader = Shader("./shaders/pbr.vert", "./shaders/pbr.frag");
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));

    glEnable(GL_DEPTH_TEST);

    std::string fpf = "./resources/pbr/";
    albedo = Texture(fpf + "rustediron2_basecolor.png", "albedo");
    normal = Texture(fpf + "rustediron2_normal.png", "normal");
    metallic = Texture(fpf + "rustediron2_metallic.png", "mettallic");
    roughness = Texture(fpf + "rustediron2_roughness.png", "roughness");
    ao = Texture(fpf + "ao.png", "ao");

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

    nrRows = 7;
    nrColumns = 7;
    spacing = 2.5;
}

void PbrRenderer::renderScene() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int width = 1280, height = 720;
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                            (float)width / height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setVec3("camPos", camera.Position);

    shader.setInt("albedoMap", 0);
    shader.setInt("normalMap", 1);
    shader.setInt("metallicMap", 2);
    shader.setInt("roughnessMap", 3);
    shader.setInt("aoMap", 4);

    glm::mat4 model;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, albedo.id());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal.id());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, metallic.id());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, roughness.id());
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, ao.id());

    // render lights and set lights uniforms
    assert(lightPositions.size() == lightColors.size());
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
    model = glm::translate(model, glm::vec3(0, 0, -5));
    model = glm::rotate(model, (float)glfwGetTime(),  glm::vec3(0, 1, 0));

    shader.setMat4("model", model);
    Primitive::renderSphere();

}
