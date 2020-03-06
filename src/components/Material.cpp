//
// Created by Krisu on 2019-12-29.
//

#include "Material.hpp"
#include "Debug.hpp"

Material::Material() {
    DEBUG_LOG("Creating Material...");
}

void Material::UpdateShaderUniform() {
    /* Binding Material Properties */
    for (int i = 0; i < materialProperties.size(); i++) {
        auto& materialProperty = materialProperties[i];
        materialProperty.SetShaderUniform(*p_shader,
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
        p_shader->Set(str, textureUnit);
    };

    unsigned tot = MaterialPropertyTypeCount;

    p_shader->UseShaderProgram();
    for (const auto &t : extra_textures) {
        bind_texture(t.name, t.texture, ++tot);
    }
}

void Material::SetShader(Shader *ns) {
    p_shader = ns;
}

void Material::SetIBLTextures(IBL const &ibl) {
    AppendTexture("ibl.irradiance", &ibl.irradiance);
    AppendTexture("ibl.prefilter", &ibl.prefilter);
    AppendTexture("ibl.brdfLUT", &ibl.brdfLUT);
}

