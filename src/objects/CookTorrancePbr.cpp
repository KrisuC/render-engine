//
// Created by Krisu on 2019-12-06.
//

#include "CookTorrancePbr.hpp"
#include "../basic/Model.hpp"

CookTorrancePbr::CookTorrancePbr() :
    Object(Shader("./shaders/cookTorrancePBR.vert",
                  "./shaders/cookTorrancePBR.frag")),
    albedo("./resources/pbr/rustediron2_basecolor.png"),
    normal("./resources/pbr/rustediron2_normal.png"),
    metallic("./resources/pbr/rustediron2_metallic.png"),
    roughness("./resources/pbr/rustediron2_roughness.png"),
    ao("./resources/pbr/ao.png") {}

void CookTorrancePbr::prepare() {
    shader.use();
    shader.setValue("albedoMap", 0);
    shader.setValue("normalMap", 1);
    shader.setValue("metallicMap", 2);
    shader.setValue("roughnessMap", 3);
    shader.setValue("aoMap", 4);
    shader.setValue("irradianceMap", 5);
    shader.setValue("prefilterMap", 6);
    shader.setValue("brdfLUT", 7);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, albedo.id());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal.id());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, metallic.id());
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, roughness.id());
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, ao.id());
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMap);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, brdfLUTTexture);
}

void CookTorrancePbr::render() {
    shader.use();
    if (!use_model) {
        Primitive::renderSphere();
    } else {
        model->render();
    }
}
