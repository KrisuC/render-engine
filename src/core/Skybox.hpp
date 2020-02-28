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
    Skybox() = default;
    explicit Skybox(const std::string& path_to_image);

    void render();

    inline IBL getIBL() { return ibl; }

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

    Texture texture {"asset/skybox/GrandCanyon.hdr", true};

    unsigned int envCubemap = 0;
    IBL ibl;
};


#endif //RENDER_ENGINE_SKYBOX_HPP
