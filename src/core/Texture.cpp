//
// Created by Krisu on 2019-11-22.
//

#include <utility>
#include <algorithm>
#include "Texture.hpp"
#include <stb_image.h>

Texture::Texture(const std::string& path, bool data_is_float):
        textureType(TextureType::Texture2D) {

    this->load_and_bind(path, data_is_float);
}

Texture::Texture(unsigned int id, TextureType type) :
        id(id), textureType(type) { }

Texture::~Texture() {
    if (id != 0) {
        glDeleteTextures(1, &id);
    }
}

void Texture::load_and_bind(const std::string &path, bool data_is_float) {
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
        throw std::runtime_error("Texture failed to load at path: "
                                 + path + "\n");
    }

    GLenum inFormat, outFormat, texelType;

    if (data_is_float) {
        inFormat = GL_RGB16F;
        outFormat = GL_RGB;
        texelType = GL_FLOAT;
    } else {
        texelType = GL_UNSIGNED_BYTE;
        if (nrComponents == 1) {
            inFormat = outFormat = GL_RED;
        } else if (nrComponents == 3) {
            inFormat = GL_SRGB;
            outFormat = GL_RGB;
        } else if (nrComponents == 4) {
            inFormat = GL_SRGB_ALPHA;
            outFormat = GL_RGBA;
        } else {
            throw std::runtime_error(
                    "Undefined texture format: nrComponents = "
                    + std::to_string(nrComponents));
        }
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    // load texture data into opengl
    glTexImage2D(GL_TEXTURE_2D, 0, inFormat, width, height, 0, outFormat,
                 texelType, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    data = nullptr;
}

Texture::Texture(Texture &&rhs) noexcept: id(rhs.id), textureType(rhs.textureType) {
    rhs.id = 0;
}

Texture &Texture::operator=(Texture &&rhs) noexcept {
    std::swap(id, rhs.id);
    std::swap(textureType, rhs.textureType);
    return *this;
}
