#include <utility>

#include <utility>

//
// Created by Krisu on 2019-11-19.
//

#include "Mesh.hpp"

Mesh::Mesh(std::vector <Vertex> vertices, std::vector<unsigned int> indices,
           std::vector <Texture> textures) {

    this->vertices = std::move(vertices);
    this->indices = std::move(indices);
    this->textures = std::move(textures);

    setupMesh();
}

void Mesh::Draw(Shader shader) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); // active proprer texture unit before binding
        // retrive texture number (the N in diffuse_textureN)
        std::stringstream ss;
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            ss << diffuseNr++;
        else if (name == "texture_specular")
            ss << specularNr++;
        else {
            std::cout << "Ues of undefined texture type " << name
                      << " in method: \"draw\"\n" << std::endl;
            exit(-1);
        }
        number = ss.str();
        shader.setInt("material." + name + number, i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
            vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // buiding EBO using indices from mFaces[]
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0); // parameter is the position.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            nullptr);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            (void *)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            (void *)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}
