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
    explicit Texture(const std::string& path,
                     bool float_data=false);

    Texture(Texture const&) = delete;
    Texture& operator=(Texture const&) = delete;

    Texture(Texture && rhs) noexcept;
    Texture& operator=(Texture && rhs) noexcept;


    explicit Texture(unsigned id, TextureType type);

    inline unsigned int ID() const { return id; }

    inline TextureType Type() const { return textureType; }

    ~Texture();

private:
    void load_and_bind(const std::string &path, bool data_is_float);

private:

    unsigned int id = 0;
    TextureType textureType = TextureType::Texture2D;
};


#endif //RENDER_ENGINE_TEXTURE_HPP
