//
// Created by Krisu on 2019-12-29.
//

#ifndef RENDER_ENGINE_PBR_MATERIAL_HPP
#define RENDER_ENGINE_PBR_MATERIAL_HPP

#include <utility>
#include <string>
#include <vector>
#include <iostream>
#include <array>
#include <memory>
#include <variant>

#include <glm/glm.hpp>

#include "MaterialProperty.hpp"
#include "Material.hpp"

class IBL;

class PBR_Material : public Material {
public:
    PBR_Material();

    // TODO: fix this macro shit
    /* Just saving me from some typing :> */
    template <MaterialPropertyType XXX, typename... Args>
    void SetMaterialProperty(Args&&... args) {
        auto xxx = static_cast<unsigned>(XXX);
        materialProperties[xxx].SetProperty(std::forward<Args>(args)...);
    }

#define __s(mp) SetMaterialProperty<MaterialPropertyType::mp>

#define SetAlbedo    __s(Albedo)
#define SetNormal    __s(Normal)
#define SetSpecular  __s(Specular)
#define SetMetallic  __s(Metallic)
#define SetRoughness __s(Roughness)
#define SetEmissive  __s(Emissive)
#define SetAo        __s(Ao)
#define SetHeight    __s(Height)

    inline void AppendTexture(const std::string &name, Texture const*t);

    void UpdateShaderUniform() override;

    void SetIBL(IBL const& ibl);

private:
    void bindTexture(const std::string &str, Texture const *texture,
            unsigned int textureUnit);;

private:
    /* All material properties */
    std::array<MaterialProperty, MaterialPropertyTypeCount> materialProperties {};

    /* Append some extra shader specific texture */
    struct ExtraTexture {
        std::string name;
        Texture const *texture;
    };
    std::vector<ExtraTexture> extra_textures {};
};








#endif //RENDER_ENGINE_PBR_MATERIAL_HPP
