//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_ENGINE_TEXTURE_HPP
#define RENDER_ENGINE_TEXTURE_HPP

#include <glad/glad.h>
#include <cassert>
#include <string>
#include "Debug.hpp"


/*
 * Resouce owner of a texture
 */

enum class TextureType {
    Texture2D = GL_TEXTURE_2D,
    CubeMap = GL_TEXTURE_CUBE_MAP
};

class Texture {
public:
    Texture() = default;
    explicit Texture(const std::string& path) {
        std::string suffix = path.substr(path.find_last_of('.'), path.size());
        if (suffix == ".png" || suffix == ".jpg" || suffix == ".tga") {
            loadWithStbimage(path, false);
        } else if (suffix == ".hdr") {
            loadWithStbimage(path, true);
        } else {
            throw std::runtime_error("not supported texture type: " + suffix);
        };
        DEBUG_LOG("Loading texture: ", path);
    }
    explicit Texture(unsigned textureName, TextureType type);

    Texture(Texture const&) = delete;
    Texture& operator=(Texture const&) = delete;

    Texture(Texture && rhs) noexcept;
    Texture& operator=(Texture && rhs) noexcept;

    inline unsigned int ID() const { return textureName; }

    inline TextureType Type() const { return textureType; }

    ~Texture();

private:
    void loadWithStbimage(const std::string& path, bool float_data);
    void loadWithGli(const std::string& path);

    unsigned int textureName = 0;
    bool gamma = false;
    TextureType textureType = TextureType::Texture2D;
};


#endif //RENDER_ENGINE_TEXTURE_HPP
