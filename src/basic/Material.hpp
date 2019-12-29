#include <utility>

#include <utility>

//
// Created by Krisu on 2019-12-29.
//

#ifndef RENDER_ENGINE_MATERIAL_HPP
#define RENDER_ENGINE_MATERIAL_HPP

#include <string>
#include <vector>
#include "basic/Texture.hpp"

// Hold and manage all the textures
// Maybe make it a virtual base class in the future
class Material {
public:
    virtual void bindTextures() = 0;
    virtual void loadTextures() = 0;

protected:
    explicit Material(std::string directory) : directory(std::move(directory)) {}
    std::string directory;
};

// General PBR material for Cook-Torrance
class PBRMaterial : public Material {
public:
    explicit PBRMaterial(std::string directory) : Material(std::move(directory)) {}
    void loadTextures() override {
        auto suffix = ".png";
        textures[albedo]    = new Texture(directory + "albedo" + suffix);
        textures[ao]        = new Texture(directory + "ao" + suffix);
        textures[metallic]  = new Texture(directory + "metallic" + suffix);
        textures[normal]    = new Texture(directory + "normal" + suffix);
        textures[roughness] = new Texture(directory + "roughness" + suffix);
        for (auto p_tex : textures) {
            p_tex->load();
        }
    }
    void bindTextures() override {
        for (int i = 0; i < textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textures[i]->bind());
        }
    }
    ~PBRMaterial() {
        for (auto p_tex : textures) {
            delete p_tex;
        }
    }

public:
    enum {
        albedo = 0, ao, metallic, normal, roughness
    };
    std::vector<Texture *> textures{5, nullptr};
};


#endif //RENDER_ENGINE_MATERIAL_HPP
