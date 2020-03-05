//
// Created by Krisu on 2019-12-29.
//

#include "Material.hpp"
#include "Debug.hpp"

Material::Material() {
    DEBUG_LOG("Creating Material...");
}

void Material::UpdateShaderUniform() {

    auto bind_texture = [&]
            (const std::string &str, Texture const *texture,
                    unsigned int textureUnit) {

        glActiveTexture(GL_TEXTURE0+textureUnit);
        glBindTexture(
                static_cast<unsigned>(texture->Type()), /* target */
                texture->ID() /* texture */
        );
        shader->Set(str, textureUnit);
    };

    shader->UseShaderProgram();
    // ------------------------------------------------------------------------
    if (map_using_status.albedo) {
        bind_texture("m.albedo.map", albedo.map, 0);
    } else {
        shader->Set("m.albedo.value", albedo.value);
    }
    // ------------------------------------------------------------------------
    if (map_using_status.metallic) {
        bind_texture("m.metallic.map", metallic.map, 1);
    } else {
        shader->Set("m.metallic.value", metallic.value);
    }
    // ------------------------------------------------------------------------
    if (map_using_status.roughness) {
        bind_texture("m.roughness.map", roughness.map, 2);
    } else {
        shader->Set("m.roughness.value", roughness.value);
    }
    // ------------------------------------------------------------------------
    if (map_using_status.emissive) {
        bind_texture("m.emissive.map", emissive.map, 3);
    } else {
        shader->Set("m.emissive.value", emissive.value);
    }
    // ------------------------------------------------------------------------
    if (map_using_status.normal) {
        bind_texture("m.normal", normal, 4);
    }
    // ------------------------------------------------------------------------
    if (ao) {
        bind_texture("m.ao", ao, 5);
    }
    // ------------------------------------------------------------------------
    if (specular) {
        bind_texture("m.specular", specular, 6);
    }
    // ------------------------------------------------------------------------
    if (height) {
        bind_texture("m.height", height, 7);
    }
    // Binding extra texture - tot 7 before extra
    int tot = 7;
    for (const auto &t : extra_textures) {
        bind_texture(t.name, t.texture, ++tot);
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


