//
// Created by Krisu on 2019-12-06.
//

#ifndef RENDER_ENGINE_COOKTORRANCEPBROBJ_HPP
#define RENDER_ENGINE_COOKTORRANCEPBROBJ_HPP

#include <Object.hpp>
#include <Shader.hpp>
#include <Mesh.hpp>

class CookTorrancePbrObj : public Object {
public:
    CookTorrancePbrObj();
    void prepare() override;
    void render() override;

private:
    Shader shader;

    Texture albedo;
    Texture normal;
    Texture metallic;
    Texture roughness;
    Texture ao;
};


#endif //RENDER_ENGINE_COOKTORRANCEPBROBJ_HPP
