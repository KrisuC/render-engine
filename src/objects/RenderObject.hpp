//
// Created by Krisu on 2019-12-09.
//

#ifndef RENDER_ENGINE_RENDEROBJECT_HPP
#define RENDER_ENGINE_RENDEROBJECT_HPP

#include <basic/Object.hpp>
#include <basic/Transform.hpp>

// Base class for all real objects for rendering
class RenderObject : public Object {
public:
    explicit RenderObject(const Shader &rs) : renderShader(rs) {}

    // setting local transformation relative to its parent
    inline void setLocalTransform(const glm::mat4 trans) {
        model = trans;
    }

    template <typename ...Args>
    inline void setShaderUnif(const std::string &name, Args &&...args) {
        renderShader.use();
        renderShader.setValue(name, std::forward<Args>(args) ...);
    }

protected:
    // The shader that is respondsible for rendering the obj
    Shader renderShader;
    Transform model;
};


#endif //RENDER_ENGINE_RENDEROBJECT_HPP
