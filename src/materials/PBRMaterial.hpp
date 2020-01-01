//
// Created by Krisu on 2019-12-30.
//

#ifndef RENDER_ENGINE_PBRMATERIAL_HPP
#define RENDER_ENGINE_PBRMATERIAL_HPP

#include "basic/Material.hpp"

// TODO: couple Material with Shader system
// General PBR material for Cook-Torrance
class PBRMaterial : public Material {
public:
    explicit PBRMaterial(std::string directory) :
        Material(std::move(directory)) {}

    void loadTextures() override;
    void bindTextures() override;
    void setShader(Shader &shader);
    void setIblPrecomptedMap(Shader &shader, unsigned int irradiance,
            unsigned int prefilter, unsigned int brdfLUT);

    ~PBRMaterial();

private:
    enum {
        albedo = 0, ao, metallic, normal, roughness
    };
    static constexpr const char* const enum_str[5] {
            "albedo_map", "ao_map", "metallic_map", "normal_map", "roughness_map"
    };
    std::vector<Texture *> textures { 5, nullptr };
};


#endif //RENDER_ENGINE_PBRMATERIAL_HPP
