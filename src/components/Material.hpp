//
// Created by Krisu on 2019-12-29.
//

#ifndef RENDER_ENGINE_MATERIAL_HPP
#define RENDER_ENGINE_MATERIAL_HPP

#include <utility>
#include <string>
#include <vector>
#include <iostream>
#include <array>
#include <memory>
#include <variant>

#include <glm/glm.hpp>

#include "Texture.hpp"
#include "IBL.hpp"
#include "Component.hpp"
#include "Shader.hpp"
#include "MaterialProperty.hpp"




class Material : public Component {
public:
    void BeforeRenderPass() override {
        UpdateShaderUniform();
    }

public:
    Material();

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

    void SetShader(Shader *ns);
    inline Shader& GetShader() { return *p_shader; }

private:
    void SetIBLTextures(IBL const& ibl);

    void UpdateShaderUniform();

    friend class Scene;

private:
    /* Shader responsible for rendering this Material */
    Shader *p_shader = &Shader::DefaultShader();

    /* All material properties */
    std::array<MaterialProperty, MaterialPropertyTypeCount> materialProperties {};

    /* Append some extra shader specific texture */
    struct ExtraTexture {
        std::string name;
        Texture const *texture;
    };
    std::vector<ExtraTexture> extra_textures {};
};







// ----------------------Inline functions----------------------------
void Material::AppendTexture(const std::string &name, Texture const *t) {
    extra_textures.push_back({name, t});
}

#endif //RENDER_ENGINE_MATERIAL_HPP
