//
// Created by Krisu on 2019-11-22.
//

#include <utility>
#include <algorithm>
#include "Texture.hpp"
#include <stb_image.h>
#include <gli/gli.hpp>

void Texture::loadWithStbimage(const std::string& path, bool data_is_float) {
    textureType = TextureType::Texture2D;
    // unsigned char (byte) for normal jpg, float for hdr image
    void *data = nullptr;

    int width, height, nrComponents;

    stbi_set_flip_vertically_on_load(true);

    if (data_is_float) {
        data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
    } else {
        data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    }
    if (!data) {
        throw std::runtime_error("Texture failed to load at path: " + path);
    }

    GLenum informat, outformat, texelType;

    if (data_is_float) {
        informat = GL_RGB16F;
        outformat = GL_RGB;
        texelType = GL_FLOAT;
    } else {
        texelType = GL_UNSIGNED_BYTE;
        if (nrComponents == 1) {
            informat = outformat = GL_RED;
        } else if (nrComponents == 3) {
            informat = GL_SRGB;
            outformat = GL_RGB;
        } else if (nrComponents == 4) {
            informat = GL_SRGB_ALPHA;
            outformat = GL_RGBA;
        } else {
            throw std::runtime_error(
                    "Undefined texture format: nrComponents = "
                    + std::to_string(nrComponents));
        }
    }

    glGenTextures(1, &textureName);
    glBindTexture(GL_TEXTURE_2D, textureName);
    // load texture data into opengl
    glTexImage2D(GL_TEXTURE_2D, 0, informat, width, height, 0, outformat,
                 texelType, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0); // unbind

    stbi_image_free(data);
    data = nullptr;
}

void Texture::loadWithGli(const std::string &path) {
    // TODO
}

Texture::Texture(unsigned int textureName, TextureType type) :
        textureName(textureName), textureType(type) { }

Texture::~Texture() {
    if (textureName != 0) {
        glDeleteTextures(1, &textureName);
    }
}


Texture::Texture(Texture &&rhs) noexcept: textureName(rhs.textureName), textureType(rhs.textureType) {
    rhs.textureName = 0;
}

Texture &Texture::operator=(Texture &&rhs) noexcept {
    std::swap(textureName, rhs.textureName);
    std::swap(textureType, rhs.textureType);
    return *this;
}


