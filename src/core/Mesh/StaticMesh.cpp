//
// Created by Krisu on 2019-11-19.
//

#include <utility>
#include <memory>
#include "StaticMesh.hpp"
#include "Debug.hpp"

StaticMesh::StaticMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
                       unsigned int mesh_type) :
            vertices(std::move(vertices)), indices(std::move(indices)),
            mesh_type(mesh_type) {
    setupMesh();
    DEBUG_LOG("Creating StaticMesh with", this->vertices.size(), "vertices...");
}

void StaticMesh::DrawCall() {
    // Render mesh
    glBindVertexArray(VAO);
    glDrawElements(mesh_type, (int)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void StaticMesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); // VBO store vertices
    glGenBuffers(1, &EBO); // EBO store indices

    glBindVertexArray(VAO);
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // Buiding EBO using indices from mFaces[]
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // Positions (location = 0)
        glEnableVertexAttribArray(0); // parameter is the position.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              nullptr);
        // Normals (location = 1)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, normal));
        // Texture coords (location = 2)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, texCoords));
    }
    glBindVertexArray(0);  // Unbind
}


namespace BuiltinMesh {
    // R = 1
    // TODO: Replace with a better Sphere
    StaticMesh& GetSphere() {
        static std::unique_ptr<StaticMesh> up_mesh;
        if (up_mesh.get() == nullptr) {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            const unsigned int X_SEGMENTS = 64;
            const unsigned int Y_SEGMENTS = 64;
            const float PI = 3.14159265359;
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
                    float xSegment = (float) x / (float) X_SEGMENTS;
                    float ySegment = (float) y / (float) Y_SEGMENTS;
                    float xPos = std::cos(xSegment*2.f*PI) * std::sin(ySegment*PI);
                    float yPos = std::cos(ySegment*PI);
                    float zPos = std::sin(xSegment*2.0f*PI) * std::sin(ySegment*PI);
                    vertices.push_back({ {xPos, yPos, zPos},
                                         {xPos, yPos, zPos},
                                         {xSegment, ySegment} });
                }
            }

            for (int y = 0; y < Y_SEGMENTS; ++y) {
                if (y % 2 == 0) {
                    for (int x = 0; x <= X_SEGMENTS; ++x) {
                        indices.push_back(y * (X_SEGMENTS + 1) + x);
                        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    }
                } else {
                    for (int x = X_SEGMENTS; x >= 0; --x) {
                        indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                        indices.push_back(y * (X_SEGMENTS + 1) + x);
                    }
                }
            }
            up_mesh = std::make_unique<StaticMesh>(std::move(vertices), std::move(indices), GL_TRIANGLE_STRIP);
        }
        return *up_mesh;
    }

    // L = 2
    StaticMesh& GetCube() {
        static std::unique_ptr<StaticMesh> up_mesh;
        if (up_mesh.get() == nullptr) {
            const static float vdata[] = {
                    // position, normal, uv
                    // back face
                    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                    1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                    1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
                    1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                    -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
                    // front face
                    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                    1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
                    1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                    1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                    -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
                    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                    // left face
                    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                    -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
                    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                    -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                    // right face
                    1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                    1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                    1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
                    1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                    1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                    1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
                    // bottom face
                    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                    1.0f, -1.0f, -1.0f,   0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
                    1.0f, -1.0f,  1.0f,   0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                    1.0f, -1.0f,  1.0f,   0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                    -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                    // top face
                    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                    1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                    1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
                    1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                    -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
            };
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            for (int i = 0; i < 36; i++) {
                vertices.push_back({ glm::make_vec3(vdata + i*8),
                                     glm::make_vec3(vdata + i*8 + 3),
                                     glm::make_vec2(vdata + i*8 + 6) });
                auto v = glm::make_vec3(vdata + i*8 + 3);
                indices.push_back(i);
            }
            up_mesh = std::make_unique<StaticMesh>(std::move(vertices), std::move(indices));
        }
        return *up_mesh;
    }

    // L = 2
    StaticMesh& GetQuad() {
        static std::unique_ptr<StaticMesh> up_mesh;
        if (up_mesh.get() == nullptr) {
            const static float vdata[] = {
                    // positions, normal, uv
                    -1.0f,  1.0f, 0.0f,  0.f, 0.f, 1.f, 0.0f, 1.0f,
                    -1.0f, -1.0f, 0.0f,  0.f, 0.f, 1.f, 0.0f, 0.0f,
                    1.0f,  1.0f, 0.0f,  0.f, 0.f, 1.f, 1.0f, 1.0f,
                    1.0f, -1.0f, 0.0f,  0.f, 0.f, 1.f, 1.0f, 0.0f,
            };
            std::vector<Vertex> vertices;
            std::vector<unsigned  int> indices { 0, 1, 2, 3 };
            vertices.reserve(4);
            for (int i = 0; i < 4; i++) {
                vertices.push_back({ glm::make_vec3(vdata + i*8),
                                     glm::make_vec3(vdata + i*8 + 3),
                                     glm::make_vec2(vdata + i*8 + 6) });
            }
            up_mesh = std::make_unique<StaticMesh>(std::move(vertices), std::move(indices), GL_TRIANGLE_STRIP);
        }
        return *up_mesh;
    }

    void RenderCube() {
        static auto s = GetSphere();
        s.DrawCall();
    }

    void RenderSphere() {
        static auto c = GetCube();
        c.DrawCall();
    }

    void RenderQuad() {
        static auto q = GetQuad();
        q.DrawCall();
    }
}