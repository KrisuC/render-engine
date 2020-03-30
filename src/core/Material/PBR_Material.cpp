//
// Created by Krisu on 2019-12-29.
//

#include "PBR_Material.hpp"
#include "Debug.hpp"
#include "Engine.hpp"

#include "Texture.hpp"
#include "IBL.hpp"
#include "Component.hpp"
#include "Shader.hpp"


PBR_Material::PBR_Material() {
    DEBUG_LOG("Creating PBR_Material...");

    SetShader(Engine::GetInstance().CreateStandardShader());

    SetAlbedo(1, 1, 1);
    SetMetallic(0);
    SetEmissive(0, 0, 0);
}

void PBR_Material::AppendTexture(const std::string &name, Texture const *t) {
    extra_textures.push_back({name, t});
}

void PBR_Material::UpdateShaderUniform() {
    /* Binding PBR_Material Properties */
    for (int i = 0; i < materialProperties.size(); i++) {
        auto& materialProperty = materialProperties[i];
        materialProperty.SetShaderUniform(GetShader(),
                static_cast<MaterialPropertyType>(i));
    }

    /* Binding extra texture */
    auto bind_texture = [&]
            (const std::string &str, Texture const *texture,
                    unsigned int textureUnit) {

        glActiveTexture(GL_TEXTURE0+textureUnit);
        glBindTexture(
                static_cast<unsigned>(texture->Type()), /* target */
                texture->ID() /* texture */
        );
        GetShader().Set(str, textureUnit);
    };

    unsigned tot = MaterialPropertyTypeCount;

    GetShader().UseShaderProgram();
    for (const auto &t : extra_textures) {
        bind_texture(t.name, t.texture, ++tot);
    }
}

void PBR_Material::SetIBLTextures(IBL const &ibl) {
    AppendTexture("ibl.irradiance", &ibl.irradiance);
    AppendTexture("ibl.prefilter", &ibl.prefilter);
    AppendTexture("ibl.brdfLUT", &ibl.brdfLUT);
}

