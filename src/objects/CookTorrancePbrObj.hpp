//
// Created by Krisu on 2019-12-06.
//

#ifndef RENDER_ENGINE_COOKTORRANCEPBROBJ_HPP
#define RENDER_ENGINE_COOKTORRANCEPBROBJ_HPP

#include "../basic/Object.hpp"
#include "../basic/Shader.hpp"
#include "../basic/Mesh.hpp"

class CookTorrancePbrObj : public Object {
public:
    CookTorrancePbrObj();
    void prepare() override;
    void render() override;
    inline void setIrradianceMap(unsigned int map) { irradianceMap = map; }
    inline void setPrefilterMap(unsigned int map) { prefilterMap = map; }
    inline void setBrdfLTUTexture(unsigned int texture) { brdfLUTTexture = texture; }

private:
    Texture albedo;
    Texture normal;
    Texture metallic;
    Texture roughness;
    Texture ao;

    unsigned int irradianceMap;
    unsigned int prefilterMap;
    unsigned int brdfLUTTexture;
};


#endif //RENDER_ENGINE_COOKTORRANCEPBROBJ_HPP
