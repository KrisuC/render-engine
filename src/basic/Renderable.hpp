//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_ENGINE_RENDERABLE_HPP
#define RENDER_ENGINE_RENDERABLE_HPP

#include "basic/Shader.hpp"
#include "basic/Transform.hpp"
#include "basic/Material.hpp"
#include "basic/Mesh.hpp"

/*
 * Renderable is an entity to be rendered in the scene.
 * A Renderable is a composite of a Mesh, a Material and a Shader who will
 * be applied to the Renderable at render pass
 */

class Renderable {
public:
    Renderable(Mesh mesh, Material material, Shader shader) :
        mesh(std::move(mesh)), material(std::move(material)), shader(shader) {}



private:
    Mesh mesh;
    Material material;
    Shader shader;
};


#endif //RENDER_ENGINE_RENDERABLE_HPP
