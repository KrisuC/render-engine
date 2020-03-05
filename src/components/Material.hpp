//
// Created by Krisu on 2019-12-29.
//

#ifndef RENDER_ENGINE_MATERIAL_HPP
#define RENDER_ENGINE_MATERIAL_HPP

#include <utility>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <glm/glm.hpp>

#include "Texture.hpp"
#include "IBL.hpp"
#include "Component.hpp"
#include "Shader.hpp"


// Hold and manage all the texture, responsible for binding texture to shader
// All suppord

class Material : public Component {
public:
    void BeforeRenderPass() override {
        UpdateShaderUniform();
    }

public:
    Material();

    inline void SetAlbedo(Texture *a);
    inline void SetAlbedo(glm::vec3 a);
    inline void SetAlbedo(float r, float g, float b);

    inline void SetMetallic(Texture *m);
    inline void SetMetallic(float m);

    inline void SetRoughness(Texture *r);
    inline void SetRoughness(float r);

    inline void SetEmissive(Texture *e);
    inline void SetEmissive(float e);

    inline void SetNormal(Texture *n);
    inline void SetAO(Texture *a);
    inline void SetSpecular(Texture *s);
    inline void SetHeight(Texture *h);

    inline void AppendTexture(const std::string &name, Texture const*t);

    void SetShader(Shader *ns);
    inline Shader& GetShader() { return *shader; }

private:
    void SetIBLTextures(IBL const& ibl);

    void UpdateShaderUniform();

    friend class Scene;

private:
    /* Shader responsible for rendering this Material */
    Shader *shader = &Shader::DefaultShader();

    // All supported general use parameter(tex/val) Type:
    struct { Texture *map; glm::vec3 value; } albedo = { nullptr, {0, 0, 0} };
    struct { Texture *map; float value; } metallic = { nullptr, 0 };
    struct { Texture *map; float value; } roughness = { nullptr, 0 };
    struct { Texture *map; float value; } emissive = { nullptr, 0 };
    Texture *normal = nullptr;
    Texture *ao = nullptr;
    Texture *specular = nullptr;
    Texture *height = nullptr;
    // Parameter status:
    struct {
        bool albedo = false;
        bool metallic = false;
        bool specular = false;
        bool roughness = false;
        bool emissive = false;
        bool normal = false;
        bool ao = false;
    } map_using_status;
    // Append some extra shader specific texture
    struct ExtraTexture {
        std::string name;
        Texture const *texture;
    };
    std::vector<ExtraTexture> extra_textures;
};







// ----------------------Inline functions----------------------------
void Material::AppendTexture(const std::string &name, Texture const *t) {
    extra_textures.push_back({name, t});
}

void Material::SetAlbedo(Texture *a)  {
    map_using_status.albedo = true; albedo.map = a; }

void Material::SetAlbedo(glm::vec3 a) { map_using_status.albedo = false; albedo.value = a; }

void Material::SetAlbedo(float r, float g, float b) { SetAlbedo({r, g, b}); }

void Material::SetMetallic(Texture *m) { map_using_status.metallic = true; metallic.map = m; }

void Material::SetMetallic(float m)    { map_using_status.metallic = false; metallic.value = m; }

void Material::SetRoughness(Texture *r) { map_using_status.roughness = true; roughness.map = r; }

void Material::SetRoughness(float r)    { map_using_status.roughness = false; roughness.value = r;; }

void Material::SetEmissive(Texture *e) { map_using_status.emissive = true; emissive.map = e; }

void Material::SetEmissive(float e)    { map_using_status.emissive = false; emissive.value = e; }

void Material::SetNormal(Texture *n) { map_using_status.normal = true ; normal = n; }

void Material::SetAO(Texture *a)     { ao = a; map_using_status.ao = true; }

void Material::SetSpecular(Texture *s) { specular = s; map_using_status.specular = true; }

void Material::SetHeight(Texture *h)   { height = h; }




#endif //RENDER_ENGINE_MATERIAL_HPP
