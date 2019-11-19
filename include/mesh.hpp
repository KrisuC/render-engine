//
// Created by Krisu on 2019-11-19.
//

#ifndef RENDER_LAB_MESH_HPP
#define RENDER_LAB_MESH_HPP

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>

#include <vector>
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>

#include "shader.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    std::string type; // a diffuse texture or a specular texture
    std::string path;
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned int> indices,
         std::vector<Texture> textures);
    void Draw(Shader shader);

    // Render data:
    unsigned int VAO, VBO, EBO;
    // Mesh data:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

private:
    void setupMesh();
};

#endif //RENDER_LAB_MESH_HPP
