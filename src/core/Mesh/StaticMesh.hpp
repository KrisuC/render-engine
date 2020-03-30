//
// Created by Krisu on 2019-11-19.
//

#ifndef RENDER_ENGINE_STATICMESH_HPP
#define RENDER_ENGINE_STATICMESH_HPP

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <vector>
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>

#include "Shader.hpp"
#include "Texture.hpp"
#include "Component.hpp"
#include "Mesh.hpp"

class StaticMesh : public Mesh {
public:
    StaticMesh() = default;
    StaticMesh(std::vector<Vertex> vertices,
               std::vector<unsigned int> indices,
               unsigned int mesh_type = GL_TRIANGLES);

    void DrawCall() override;

public:
    unsigned int VAO = 0, VBO = 0, EBO = 0;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int material_index = -1;

private:
    // Called in constructor
    void setupMesh();

    unsigned int mesh_type = GL_TRIANGLES;
};



// Rendering some fixed size primitive
namespace BuiltinMesh {
    // Unit == 2 :)
    StaticMesh& GetSphere();
    StaticMesh& GetCube();
    StaticMesh& GetQuad();

    void RenderCube();
    void RenderSphere();
    void RenderQuad();
}

#endif //RENDER_ENGINE_STATICMESH_HPP
