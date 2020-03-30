//
// Created by Krisu on 2020/3/30.
//

#ifndef RENDER_ENGINE_MESHRENDERER_HPP
#define RENDER_ENGINE_MESHRENDERER_HPP

#include "Component.hpp"
#include "Mesh.hpp"
#include "PBR_Material.hpp"

class MeshRenderer : public Component {
public:
    MeshRenderer(Mesh &mesh, Material &material) : mesh(mesh),
                                                   material(material) {}

    void Update() override {
        material.UpdateShaderUniform();
        material.GetShader().UseShaderProgram();
        mesh.DrawCall();
    }

public:
    Mesh& mesh;
    Material& material;
};

#endif //RENDER_ENGINE_MESHRENDERER_HPP
