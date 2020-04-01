//
// Created by Krisu on 2020/3/30.
//

#include "ModelManager.hpp"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Transform.hpp"
#include "MeshRenderer.hpp"

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
    m_meshes.resize(pScene->mNumMeshes);
    m_materials.resize(pScene->mNumMaterials);

    for (int index = 0; index < m_meshes.size(); index++) {
        const aiMesh *paiMesh = pScene->mMeshes[index];
        initMesh(index, paiMesh);
    }
    return initMaterials(pScene, file_name);
}


void ModelManager::initMesh(int m_meshes_index, const aiMesh *p_aiMesh) {
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

    m_meshes[m_meshes_index] = new StaticMesh {std::move(vertices), std::move(indicies)};
    m_meshes[m_meshes_index]->material_index = p_aiMesh->mMaterialIndex;
}


bool
ModelManager::initMaterials(const aiScene *p_aiScene, const std::string &file_name) {
    std::string::size_type slash_pos = file_name.find_last_of('/');

    if (slash_pos == std::string::npos) {
        directory = ".";
    } else if (slash_pos == 0) {
        directory = "/";
    } else {
        directory = file_name.substr(0, slash_pos);
    }

    DEBUG_LOG("total materials for model:", p_aiScene->mNumMaterials);
    for (int i = 0; i < p_aiScene->mNumMaterials; i++) {
        const aiMaterial *p_aiMaterial = p_aiScene->mMaterials[i];

        m_materials[i] = new PBR_Material;
        m_materials[i]->SetShader(Engine::GetInstance().GetStandardShader());
        m_materials[i]->SetIBL(Engine::GetInstance().GetCurrentScene().GetSkybox().GetIBL());

        // !!! Only work for sponzaPBR !!!
        if (Texture *t_albedo = initMaterialTexture(p_aiMaterial, aiTextureType_DIFFUSE)) {
            DEBUG_LOG("Albedo texture loaded");
            m_materials[i]->SetAlbedo(t_albedo);
        }
        if (Texture *t_metallic = initMaterialTexture(p_aiMaterial, aiTextureType_AMBIENT)) {
            DEBUG_LOG("Metallic texture loaded");
            m_materials[i]->SetMetallic(t_metallic);
        }
        if (Texture *t_roughness = initMaterialTexture(p_aiMaterial, aiTextureType_SHININESS)) {
            DEBUG_LOG("Roughness texture loaded");
            m_materials[i]->SetRoughness(t_roughness);
        }
        if (Texture *t_normal = initMaterialTexture(p_aiMaterial, aiTextureType_DISPLACEMENT)) {
            DEBUG_LOG("Normal texture loaded");
            m_materials[i]->SetNormal(t_normal);
        }
    }
    return true;
}


Texture* ModelManager::initMaterialTexture(const aiMaterial* p_aiMaterial,
                                           aiTextureType textureType) {
    if (p_aiMaterial->GetTextureCount(textureType) > 0) {
        // now only 1 texture for each material property is supported...
        assert(p_aiMaterial->GetTextureCount(textureType) == 1);
        aiString relative_path;
        // Get texture of this material
        if (p_aiMaterial->GetTexture(textureType, 0, &relative_path,
                                     nullptr, nullptr, nullptr, nullptr,
                                     nullptr) == AI_SUCCESS) {
            /* fix relative path */
            for (int i = 0; i < relative_path.length; i++) {
                if (relative_path.data[i] == '\\') {
                    relative_path.data[i] = '/';
                }
            }
            std::string full_path = directory + "/" + relative_path.data;
            return new Texture(full_path);
        }
    }
    return nullptr;
}


ModelManager::~ModelManager() {
    for (auto &p : m_meshes) {
        delete p;
    }
    for (auto &p : m_materials) {
        delete p;
    }
}


