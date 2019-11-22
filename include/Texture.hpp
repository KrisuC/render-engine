//
// Created by Krisu on 2019-11-22.
//

#ifndef RENDER_LAB_TEXTURE_HPP
#define RENDER_LAB_TEXTURE_HPP

#include <glad/glad.h>
#include <string>

class Texture {
public:
    explicit Texture(std::string path, std::string tag="diffuse");

    unsigned bindTexture();
    void loadTexture();

    inline unsigned id() { return textureID; };
    inline std::string type() { return tag; }

    ~Texture();

private:
    unsigned char *data = nullptr;
    std::string filePath;
    std::string tag;
    unsigned int textureID = -1;
    int width = 0, height = 0, nrComponents = 0;
};


#endif //RENDER_LAB_TEXTURE_HPP
