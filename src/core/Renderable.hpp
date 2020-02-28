//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_ENGINE_RENDERABLE_HPP
#define RENDER_ENGINE_RENDERABLE_HPP

#include "Shader.hpp"
#include "Transform.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include <memory>

/*
 * Renderable is an entity to be rendered in the scene.
 * A Renderable is a composite of a Mesh, a Material and a Shader who will
 * be applied to the Renderable at render pass
 *
 * Mesh, Material and Shader are NOT maintained by Renderable but by other
 * classes (maybe Scene or some user-defined type)
 */

class Renderable {
public:
    Renderable(Mesh *mesh, Material *material, Shader *shader) :
        mesh(mesh), material(material), shader(shader) {}

private:
    Mesh *mesh;
    Material *material;
    Shader *shader;

    friend class Scene;
};


#endif //RENDER_ENGINE_RENDERABLE_HPP
