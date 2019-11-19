//
// Created by Krisu on 2019-11-18.
//

#ifndef RENDER_LAB_MODEL_HPP
#define RENDER_LAB_MODEL_HPP

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <iostream>
#include <cstdio>
#include <string>

#include "shader.hpp"
#include "mesh.hpp"

unsigned int TextureFromFile(char const * path, const std::string & directory);
unsigned int loadTexture(char const * path);

class Model {
public:
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    // functions:
    explicit Model (const std::string & path);
    void Draw(Shader shader);

private:
    std::string directory;

    void loadModel(const std::string &path);
    void processNode(aiNode * node, const aiScene * scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial * mat,
                                              aiTextureType type,
                                              const std::string& typeName);
    Mesh processMesh(aiMesh * mesh, const aiScene * scene);
};



#endif //RENDER_LAB_MODEL_HPP
