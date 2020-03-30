//
// Created by Krisu on 2020/3/30.
//

#ifndef RENDER_ENGINE_MESH_HPP
#define RENDER_ENGINE_MESH_HPP

#include <glm/glm.hpp>

class Vertex {
public:
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
public:
    virtual void DrawCall() = 0;
};


#endif //RENDER_ENGINE_MESH_HPP
