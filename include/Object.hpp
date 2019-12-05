//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_ENGINE_OBJECT_HPP
#define RENDER_ENGINE_OBJECT_HPP

#include <Shader.hpp>
#include <Model.hpp>
#include <memory>

class Object {
public:
    virtual void prepare() = 0;
    virtual void render() = 0;
    template <typename T>
    inline void setShaderValue(const std::string &name, const T &value) {
        shader.setValue(name, value);
    }

public:
    Shader shader;
};


#endif //RENDER_ENGINE_OBJECT_HPP
