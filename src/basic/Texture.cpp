//
// Created by Krisu on 2019-11-22.
//

#include <utility>
#include <iostream>
#include "Texture.hpp"
#include <stb_image.h>

Texture::Texture(std::string textureFilePath, std::string tag) :
        filePath(std::move(textureFilePath)), tag(std::move(tag)) {
    // Do nothing
    // Load and bind must be called explicitly
}


void Texture::load() {
    stbi_set_flip_vertically_on_load(true);

    if (tag == "hdr") {
        data = stbi_loadf(filePath.c_str(), &width, &height, &nrComponents, 0);
    } else {
        data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);
    }
    if (!data) {
        throw std::runtime_error("Texture failed to load at path: "
                                 + filePath + "\n");
    }
}

unsigned Texture::bind() {
    // Load and bind if not
    if (textureID == 0) {
        GLenum inFormat, outFormat, texelType;

        if (tag == "hdr") {
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
                         + std::to_string(nrComponents) + " for " + filePath);
            }
        }

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        // load texture data into opengl
        glTexImage2D(GL_TEXTURE_2D, 0, inFormat, width, height, 0, outFormat,
                     texelType, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        free();
    }
    return textureID;
}

Texture::~Texture() {
    free();
    // delete texture from opengl and free space
    glDeleteTextures(1, &textureID);
}

void Texture::free() {
    if (data) {
        stbi_image_free(data);
        data = nullptr;
    }
}
