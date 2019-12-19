//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_ENGINE_OBJECT_HPP
#define RENDER_ENGINE_OBJECT_HPP

#include "basic/Shader.hpp"
#include "basic/Transform.hpp"
#include "basic/Geometry.hpp"

// Represents an entity in scene
// TODO: make Object an entity that holds Mesh, Shaders, Transform,
//       Textures.
class Object {
public:
    virtual void prepare() = 0;
    virtual void render() = 0;
    virtual ~Object() = default;

public:
    explicit Object(const Shader &rs) : shader(rs) {}

    // setting local transformation relative to its parent
    inline void setLocalTransform(const Transform &t) {
        localTransfrom = t;
    }

    template <typename ...Args>
    inline void setShaderUnif(const std::string &name, Args &&...args) {
        shader.use();
        shader.setValue(name, std::forward<Args>(args) ...);
    }

protected:
    // The shader that is respondsible for rendering the object
    Shader shader;
    // The world transformation of the current object is the parent's
    //  world transformation multiplying the object's local transformation
    Transform localTransfrom;
    // store triangular meshes
    Geometry geometry;
};


#endif //RENDER_ENGINE_OBJECT_HPP
