//
// Created by Krisu on 2019-11-18.
//

#include "model.hpp"
#include <stb_image.h>

void Model::loadModel(const std::string &path) {
    Assimp::Importer import;
    const aiScene * scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

    // check root node and flags of imcomplete after import
    if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        exit(-1);
    }
    directory = path.substr(0, path.find_last_of('/'));

    // pass first node (root node) to recursive processNode
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // process all the node's meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // do the same for each child
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Model::Model(const std::string &path) {
    loadModel(path);
}

void Model::Draw(Shader shader) {
    for (auto & mesh : meshes) {
        mesh.Draw(shader);
    }
}

std::vector<Texture> Model::loadMaterialTextures(
        aiMaterial *mat,
        aiTextureType type,
        const std::string& typeName) {

    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (auto & j : textures_loaded) {
            if (std::strcmp(j.path.c_str(), str.C_Str()) == 0) {
                textures.push_back(j);
                skip = true;
                break;
            }
        }
        if (!skip) {
            // not already loaded before
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex{};
        // process each vertex positions
        glm::vec3 tempVector;
        // position:
        tempVector.x = mesh->mVertices[i].x;
        tempVector.y = mesh->mVertices[i].y;
        tempVector.z = mesh->mVertices[i].z;
        vertex.Position = tempVector;
        // normal:
        tempVector.x = mesh->mNormals[i].x;
        tempVector.y = mesh->mNormals[i].y;
        tempVector.z = mesh->mNormals[i].z;
        vertex.Normal = tempVector;
        // texture coordinates
        if (mesh->mTextureCoords[0]) { // contain?
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;

            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0, 0.0f);

        vertices.push_back(vertex);
    }
    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace & face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // process material
    aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    return Mesh(vertices, indices, textures);
}

unsigned int TextureFromFile(char const *path, const std::string &directory) {
    std::string filename = std::string(path);
    if (filename.find('\\') != filename.size()) { // deal with some bad design model
        filename = filename.substr(filename.find_last_of('\\') + 1, filename.size());
    }
    filename = directory + '/' + filename;


    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(),
                                    &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1) {
            format = GL_RED;
        } else if (nrComponents == 3) {
            format = GL_RGB;
        }
        else if (nrComponents == 4) {
            format = GL_RGBA;
        } else {
            std::cerr << "Undefined channel number.\n";
            exit(-1);
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
        exit(-1);
    }
    return textureID;
}
