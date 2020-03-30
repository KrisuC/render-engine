//
// Created by Krisu on 2020/3/30.
//

#include "ModelManager.hpp"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

bool ModelManager::LoadModel(const std::string &file_name) {
    bool ret = false;
    Assimp::Importer importer;

    const aiScene *pScene = importer.ReadFile(file_name.c_str(),
                                              aiProcess_Triangulate |
                                              aiProcess_FlipUVs |
                                              aiProcess_JoinIdenticalVertices);

    if (pScene) {
        ret = initFromScene(pScene, file_name);
    } else {
        std::cerr << "Error parsing " << file_name << " : "
                  << importer.GetErrorString() << std::endl;
    }
    return ret;
}

bool
ModelManager::initFromScene(const aiScene *pScene, const std::string &file_name) {
    p_meshes.resize(pScene->mNumMeshes);
    p_textures.resize(pScene->mNumMaterials);

    for (int index = 0; index < p_meshes.size(); index++) {
        const aiMesh *paiMesh = pScene->mMeshes[index];
        initMesh(p_meshes[index], paiMesh);
    }
    return initMaterials(pScene, file_name);
}

void ModelManager::initMesh(StaticMesh *p_mesh, const aiMesh *p_aiMesh) {
    // Assign material to mesh

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indicies;

    static const aiVector3D zero_vec3{0.0f};

    for (int i = 0; i < p_aiMesh->mNumVertices; i++) {
        const aiVector3D *pPos = &(p_aiMesh->mVertices[i]);
        const aiVector3D *pNormal = p_aiMesh->HasNormals()
                                    ? &(p_aiMesh->mNormals[i]) : &zero_vec3;
        const aiVector3D *pTexCoord = p_aiMesh->HasTextureCoords(0)
                                      ? &(p_aiMesh->mTextureCoords[0][i])
                                      : &zero_vec3;
        Vertex v{glm::vec3(pPos->x, pPos->y, pPos->z),
                 glm::vec3(pNormal->x, pNormal->y, pNormal->z),
                 glm::vec2(pTexCoord->x, pTexCoord->y)};

        vertices.push_back(v);
    }

    for (int i = 0; i < p_aiMesh->mNumFaces; i++) {
        const aiFace &face = p_aiMesh->mFaces[i];
        assert(face.mNumIndices == 3);
        for (int j = 0; j < 3; j++) {
            indicies.push_back(face.mIndices[j]);
        }
    }

    DEBUG_LOG("Generating mesh...", vertices.size(), "vertices. ",
              indicies.size(), "indicies");

    p_mesh = new StaticMesh {std::move(vertices), std::move(indicies)};
    p_mesh->material_index = p_aiMesh->mMaterialIndex;
}

bool
ModelManager::initMaterials(const aiScene *pScene, const std::string &file_name) {
    std::string::size_type slash_pos = file_name.find_last_of('/');
    std::string directory;

    if (slash_pos == std::string::npos) {
        directory = ".";
    } else if (slash_pos == 0) {
        directory = "/";
    } else {
        directory = file_name.substr(0, slash_pos);
    }

    for (int i = 0; i < pScene->mNumMaterials; i++) {
        const aiMaterial *pMaterial = pScene->mMaterials[i];

        p_textures[i] = nullptr;
        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {

            aiString relative_path;
            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &relative_path,
                                      nullptr, nullptr, nullptr, nullptr,
                                      nullptr) == AI_SUCCESS) {
                std::string full_path = directory + "/" + relative_path.data;
                DEBUG_LOG("Loading diffuse texture...", full_path);
                p_textures[i] = new Texture(full_path, false);
            }
        }
    }
    return true;
}

ModelManager::~ModelManager() {
    for (auto &p : p_meshes) {
        delete p;
    }
    for (auto &p : p_textures) {
        delete p;
    }
}
