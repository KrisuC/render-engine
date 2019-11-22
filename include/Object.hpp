//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_LAB_OBJECT_HPP
#define RENDER_LAB_OBJECT_HPP

#include <Shader.hpp>
#include <Model.hpp>
#include <memory>

class Object {
public:
    Object() = default;
    void draw();

private:
    std::unique_ptr<Shader> pShader;
    std::unique_ptr<Model> pModel;
};


#endif //RENDER_LAB_OBJECT_HPP
