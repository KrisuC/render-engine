//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_ENGINE_OBJECT_HPP
#define RENDER_ENGINE_OBJECT_HPP

#include "basic/Shader.hpp"
#include "basic/Transform.hpp"

// Represents an entity in scene
class Object {
public:
    virtual void prepare() = 0;
    virtual void render() = 0;
    virtual ~Object() = default;

public:
    explicit Object(const Shader &rs) : renderShader(rs) {}

    // setting local transformation relative to its parent
    inline void setLocalTransform(const glm::mat4 t) {
        localTransfrom = t;
    }

    template <typename ...Args>
    inline void setShaderUnif(const std::string &name, Args &&...args) {
        renderShader.use();
        renderShader.setValue(name, std::forward<Args>(args) ...);
    }

protected:
    // The shader that is respondsible for rendering the object
    Shader renderShader;
    // The world transformation of the current object is the parent's
    // world transformation multiplying the object's local transformation
    Transform localTransfrom;
};


#endif //RENDER_ENGINE_OBJECT_HPP
