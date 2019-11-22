//
// Created by Krisu on 2019-11-22.
//

#include <utility>
#include <iostream>
#include <Texture.hpp>
#include <stb_image.h>

void Texture::loadTexture() {
    data = stbi_load(filePath.c_str(),
                     &width, &height, &nrComponents, 0);

    if (!data) {
        std::cerr << "Texture failed to load at path: "
                  << filePath << std::endl;
        exit(-1);
    }
}

unsigned Texture::bindTexture() {
    GLenum inFormat, outFormat;
    if (nrComponents == 1) {
        inFormat = outFormat = GL_RED;
    } else if (nrComponents == 3) {
        inFormat = GL_SRGB;
        outFormat = GL_RGB;
    } else if (nrComponents == 4) {
        inFormat = GL_SRGB_ALPHA;
        outFormat = GL_RGBA;
    } else {
        std::cerr << "Undefined texture format\n";;
        exit(-1);
    }

    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, inFormat, width, height, 0, outFormat,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}

Texture::Texture(std::string path, std::string tag) :
    filePath(std::move(path)), tag(std::move(tag)) {}

Texture::~Texture() {
    stbi_image_free(data);
}
