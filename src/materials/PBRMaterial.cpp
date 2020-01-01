//
// Created by Krisu on 2019-12-30.
//

#include "PBRMaterial.hpp"

const char* const PBRMaterial::enum_str[5];

void PBRMaterial::loadTextures() {
    for (int i = 0; i < 5; i++) {
        textures[i] = new Texture(directory + enum_str[i] + ".png");
        textures[i]->load();
    }
}

void PBRMaterial::bindTextures() {
    for (int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]->bind());
    }
}

void PBRMaterial::setShader(Shader &shader) {
    shader.use();
    for (int i = 0; i < textures.size(); i++) {
        shader.set(enum_str[i], i);
    }
}

PBRMaterial::~PBRMaterial() {
    for (auto p_tex : textures) {
        delete p_tex;
    }
}

void PBRMaterial::setIblPrecomptedMap(Shader &shader, unsigned int irradiance,
                                      unsigned int prefilter,
                                      unsigned int brdfLUT) {
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, irradiance);
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, prefilter);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, brdfLUT);
    shader.use();
    shader.set("irradianceMap", 5);
    shader.set("prefilterMap", 6);
    shader.set("brdfLUT", 7);
}
