//
// Created by Krisu on 2019-12-29.
//

#include "Material.hpp"
#include "Debug.hpp"

Material::Material() {
    DEBUG_LOG("Creating Material...");
}

void Material::UpdateShaderUniform() {
    current_used_texture_units = 1;

    auto bind_texture = [&]
            (const std::string &str, Texture const *texture) {
        glActiveTexture(GL_TEXTURE0+current_used_texture_units);
        glBindTexture(
                static_cast<unsigned>(texture->Type()),
                texture->ID()
        );
        shader->Set(str, current_used_texture_units);
        current_used_texture_units++;
    };

    shader->UseShaderProgram();
    // ------------------------------------------------------------------------
    if (map_using_status.albedo) {
        bind_texture("m.albedo.map", albedo.map);
    } else {
        shader->Set("m.albedo.value", albedo.value);
    }
    // ------------------------------------------------------------------------
    if (map_using_status.metallic) {
        bind_texture("m.metallic.map", metallic.map);
    } else {
        shader->Set("m.metallic.value", metallic.value);
    }
    // ------------------------------------------------------------------------
    if (map_using_status.roughness) {
        bind_texture("m.roughness.map", roughness.map);
    } else {
        shader->Set("m.roughness.value", roughness.value);
    }
    // ------------------------------------------------------------------------
    if (map_using_status.emissive) {
        bind_texture("m.emissive.map", emissive.map);
    } else {
        shader->Set("m.emissive.value", emissive.value);
    }
    // ------------------------------------------------------------------------
    if (map_using_status.normal) {
        bind_texture("m.normal", normal);
    }
    // ------------------------------------------------------------------------
    if (ao) {
        bind_texture("m.ao", ao);
    }
    // ------------------------------------------------------------------------
    if (specular) {
        bind_texture("m.specular", specular);
    }
    // ------------------------------------------------------------------------
    if (height) {
        bind_texture("m.height", height);
    }
    // Binding extra texture
    for (const auto &t : extra_textures) {
        bind_texture(t.name, t.texture);
    }
}

void Material::SetShader(Shader *ns) {
    shader = ns;
}

void Material::SetIBLTextures(IBL const &ibl) {
    AppendTexture("ibl.irradiance", &ibl.irradiance);
    AppendTexture("ibl.prefilter", &ibl.prefilter);
    AppendTexture("ibl.brdfLUT", &ibl.brdfLUT);
}


