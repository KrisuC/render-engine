//
// Created by Krisu on 2020/3/30.
//

#ifndef RENDER_ENGINE_MODELMANAGER_HPP
#define RENDER_ENGINE_MODELMANAGER_HPP

#include <string>
#include "StaticMesh.hpp"
#include "Texture.hpp"
#include "PBR_Material.hpp"
#include "GameObject.hpp"
#include "Scene.hpp"
#include <assimp/material.h>

class aiScene;

class aiMesh;

class aiScene;

class aiMaterial;

class ModelManager {
public:
    ModelManager() = default;

    ~ModelManager();

    bool LoadModel(const std::string &file_name);

private:
    bool initFromScene(const aiScene *pScene, const std::string &file_name);

    void initMesh(int m_meshes_index, const aiMesh *p_aiMesh);

    bool initMaterials(const aiScene *p_aiScene, const std::string &file_name);

    Texture* initMaterialTexture(const aiMaterial* p_aiMaterial, aiTextureType textureType);

public:
    std::vector<StaticMesh *> m_meshes;
    std::vector<PBR_Material *> m_materials;

private:
    std::string directory;
};


#endif //RENDER_ENGINE_MODELMANAGER_HPP
