//
// Created by Krisu on 2019-12-29.
//

#ifndef RENDER_ENGINE_MATERIAL_HPP
#define RENDER_ENGINE_MATERIAL_HPP

#include <utility>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "basic/Texture.hpp"
#include "basic/Shader.hpp"

// Hold and manage all the textures
// Maybe make it a virtual base class in the future
class Material {
public:
    Material() = default;
    explicit Material(std::string directory) : directory(std::move(directory)) {}

private:
    std::string directory;

    // All supported texture/parameter type:
    static constexpr const int t_size = 8;
    enum {
        albedo = 0, metallic, specular, roughness, normal, ao, emissive, height
    };
    static constexpr const char* const to_str[t_size] {
        "albedo", "metallic", "specular", "roughness", "normal", "ao",
        "emissive", "height"
    };
    Texture* textures[t_size] { nullptr }; // all null by default
    glm::vec3 albedo_val {1, 1, 1};
    float     metallic_val {0};
    float     specular_val {0};
    float     roughness_val {0};

public:
    // Shader responsible for this texture
    Shader *shader = new Shader{"shaders/cook-torrance.vert", "shaders/cook-torrance.frag"};
};

#endif //RENDER_ENGINE_MATERIAL_HPP
