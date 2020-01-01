//
// Created by Krisu on 2019-12-06.
//

#include "CookTorrancePBR.hpp"
#include "engine/Model.hpp"

CookTorrancePBR::CookTorrancePBR() :
    Object(Shader("./shaders/cook-torrance.vert",
                  "./shaders/cook-torrance.frag")) {}

void CookTorrancePBR::prepare() {
    shader.use();
    shader.set("albedoMap", 0);
    shader.set("normalMap", 1);
    shader.set("metallicMap", 2);
    shader.set("roughnessMap", 3);
    shader.set("aoMap", 4);
    shader.set("irradianceMap", 5);
    shader.set("prefilterMap", 6);
    shader.set("brdfLUT", 7);

    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
}

void CookTorrancePBR::render() {
    shader.use();
    if (!use_model) {
        Primitive::unitSphere();
    } else {
        model->render();
    }
}
