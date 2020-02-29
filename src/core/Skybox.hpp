//
// Created by Krisu on 2019-12-06.
//

#ifndef RENDER_ENGINE_SKYBOX_HPP
#define RENDER_ENGINE_SKYBOX_HPP

#include "GameObject.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "IBL.hpp"

// TODO: store precomputed map into actual files.
class Skybox {
public:
    Skybox();
    explicit Skybox(Texture const& skyboxTexture);

    void Render();

    inline IBL const& GetIBL() { return ibl; }

private:
    void prepare();

private:
    Shader shader {"shader/skybox.vert",
                   "shader/skybox.frag"};

    Shader equirectToCubemapShader {"shader/cubemap.vert",
                                    "shader/equirectangular-to-cubemap.frag"};

    Shader irradianceShader {"shader/cubemap.vert",
                             "shader/irradiance-convolution.frag"};

    Shader prefilterShader {"shader/cubemap.vert",
                            "shader/prefilter-map.frag"};

    Shader brdfLUTShader {"shader/brdf.vert",
                          "shader/brdf.frag"};

    unsigned int envCubemap = 0;
    Texture const& texture;

    IBL ibl;
};


#endif //RENDER_ENGINE_SKYBOX_HPP
