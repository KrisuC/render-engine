//
// Created by Krisu on 2020/3/30.
//

#ifndef RENDER_ENGINE_MODELMANAGER_HPP
#define RENDER_ENGINE_MODELMANAGER_HPP

#include <string>
#include "Mesh.hpp"
#include "Texture.hpp"

class aiScene;
class aiMesh;
class aiScene;

class ModelManager {
public:
    ModelManager() = default;

    ~ModelManager();

    bool LoadModel(const std::string &file_name);


private:
    bool initFromScene(const aiScene *pScene, const std::string &file_name);


    static void initMesh(Mesh *p_mesh, const aiMesh *p_aiMesh);


    bool initMaterials(const aiScene *pScene, const std::string &file_name);


public:
    std::vector<Mesh*> p_meshes;
    std::vector<Texture*> p_textures;
};


#endif //RENDER_ENGINE_MODELMANAGER_HPP
