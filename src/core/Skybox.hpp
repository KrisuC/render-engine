//
// Created by Krisu on 2019-12-06.
//

#ifndef RENDER_ENGINE_SKYBOX_HPP
#define RENDER_ENGINE_SKYBOX_HPP

#include "GameObject.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "IBL.hpp"


// TODO: rewrite skybox
class Skybox {
public:
    Skybox();
    explicit Skybox(Texture const& skyboxTexture);

    Skybox(Skybox const&) = delete;
    Skybox& operator=(Skybox const&) = delete;

    void Render();

    inline IBL const& GetIBL() { return ibl; }

private:
    Shader renderShader {"shader/skybox-and-IBL/skybox.vert",
                         "shader/skybox-and-IBL/skybox.frag"};

    Shader equirectToCubemapShader {"shader/skybox-and-IBL/cubemap.vert",
                                    "shader/skybox-and-IBL/equirectangular-to-cubemap.frag"};

    Shader irradianceShader {"shader/skybox-and-IBL/cubemap.vert",
                             "shader/skybox-and-IBL/irradiance-convolution.frag"};

    Shader prefilterShader {"shader/skybox-and-IBL/cubemap.vert",
                            "shader/skybox-and-IBL/prefilter-map.frag"};

    Shader brdfLUTShader {"shader/skybox-and-IBL/brdflut.vert",
                          "shader/skybox-and-IBL/brdflut.frag"};

    unsigned int envCubemap = 0;

    /* Skybox Texture */
    Texture const& texture;

    IBL ibl;
};


#endif //RENDER_ENGINE_SKYBOX_HPP
