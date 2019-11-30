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
    Object() = default;

    std::unique_ptr<Shader> pShader;
    std::unique_ptr<Model> pModel;
    std::vector<std::unique_ptr<Texture>> vpTextures;
};


#endif //RENDER_ENGINE_OBJECT_HPP
