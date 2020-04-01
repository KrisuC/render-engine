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


class aiScene;

class aiMesh;

class aiScene;

class ModelManager {
public:
    ModelManager() = default;

    ~ModelManager();

    bool LoadModel(const std::string &file_name);

    void Generate(Scene &scene);

private:
    bool initFromScene(const aiScene *pScene, const std::string &file_name);


    void initMesh(int mesh_index, const aiMesh *p_aiMesh);


    bool initMaterials(const aiScene *pScene, const std::string &file_name);

public:
    std::vector<StaticMesh *> m_meshes;
    std::vector<PBR_Material *> m_materials;
};


#endif //RENDER_ENGINE_MODELMANAGER_HPP
