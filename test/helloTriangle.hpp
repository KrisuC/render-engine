//
// Created by Krisu on 2019-10-27.
//

#ifndef RENDER_LAB_HELLOTRIANGLE_HPP
#define RENDER_LAB_HELLOTRIANGLE_HPP

#include "renderer.hpp"
#include "shader.hpp"

// This is the helloworld scnene in cg which output a single moving triangle
class TriangleTestRenderer : public Renderer {
public:
    TriangleTestRenderer();
    ~TriangleTestRenderer() = default;
    void render() override;

private:
    unsigned int sp = 0;
    unsigned int VAO = 0, VBO = 0;
};

TriangleTestRenderer::TriangleTestRenderer() {
    Shader shaderProgramObject = Shader("shaders/triangle.vert",
                                        "shaders/triangle.frag");
    sp = shaderProgramObject.getID();

    float vertices[] = {
            // posision              color
            -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f,       0.0f, 1.0f, 0.0f,
            0.5, -0.5, 0.0f,        0.0f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void TriangleTestRenderer::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // new add
    float timeValue = glfwGetTime();
    float offsetValue = (sin(timeValue * 2.0) / 2.0f);
    int vertexOffsetLocation = glGetUniformLocation(sp, "offset");

    glUseProgram(sp);
    // updating uniform value must be after using the shader program
    glUniform1f(vertexOffsetLocation, offsetValue);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
}

#endif //RENDER_LAB_HELLOTRIANGLE_HPP
