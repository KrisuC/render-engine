//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_ENGINE_GAMEOBJECT_HPP
#define RENDER_ENGINE_GAMEOBJECT_HPP

#include "Shader.hpp"
#include "Transform.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include <memory>

/*
 * GameObject is an entity to be rendered in the scene.
 * A GameObject is a composite of a Mesh, a Material and a Shader who will
 * be applied to the GameObject at render pass
 *
 * Mesh, Material and Shader are NOT maintained by GameObject but by other
 * classes (maybe Scene or some user-defined type)
 */

class GameObject {
public:
    GameObject(Mesh *mesh, Material *material) :
        mesh(mesh), material(material) {}

private:
    Mesh *mesh;
    Material *material;

    friend class Scene;
};


#endif //RENDER_ENGINE_GAMEOBJECT_HPP
