//
// Created by Krisu on 2019-12-29.
//

#include "Material.hpp"

void Material::UpdateShader() {
    current_used_texture_units = 1;

    auto bind_texture = [&]
            (const std::string &str, Texture *texture) {
        glActiveTexture(GL_TEXTURE0+current_used_texture_units);
        glBindTexture(texture->Type(), texture->Bind());
        shader->Set(str, current_used_texture_units);
        current_used_texture_units++;
    };

    shader->UseShaderProgram();
    // ------------------------------------------------------------------------
    if (map_using_status.albedo) {
        bind_texture("albedo.map", albedo.map);
    } else {
        shader->Set("albedo.value", albedo.value);
    }
    // ------------------------------------------------------------------------
    if (map_using_status.metallic) {
        bind_texture("metallic.map", metallic.map);
    } else {
        shader->Set("metallic.value", metallic.value);
    }
    // ------------------------------------------------------------------------
    if (map_using_status.roughness) {
        bind_texture("roughness.map", roughness.map);
    } else {
        shader->Set("roughness.value", roughness.value);
    }
    // ------------------------------------------------------------------------
    if (map_using_status.emissive) {
        bind_texture("emissive.map", emissive.map);
    } else {
        shader->Set("emissive.value", emissive.value);
    }
    // ------------------------------------------------------------------------
    if (map_using_status.normal) {
        bind_texture("normal", normal);
    }
    // ------------------------------------------------------------------------
    if (ao) {
        bind_texture("ao", ao);
    }
    // ------------------------------------------------------------------------
    if (specular) {
        bind_texture("specular", specular);
    }
    // ------------------------------------------------------------------------
    if (height) {
        bind_texture("height", height);
    }
    // Binding extra texture
    for (const auto &t : extra_textures) {
        bind_texture(t.name, t.texture);
    }
}

void Material::SetShader(Shader *ns) {
    shader = ns;
}


