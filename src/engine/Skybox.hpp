//
// Created by Krisu on 2019-12-06.
//

#ifndef RENDER_ENGINE_SKYBOX_HPP
#define RENDER_ENGINE_SKYBOX_HPP

#include "Renderable.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "engine/IBL.hpp"

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
    Shader shader {"shaders/skybox.vert",
                   "shaders/skybox.frag"};

    Shader equirectToCubemapShader {"shaders/cubemap.vert",
                                    "shaders/equirectangular-to-cubemap.frag"};

    Shader irradianceShader {"shaders/cubemap.vert",
                             "shaders/irradiance-convolution.frag"};

    Shader prefilterShader {"shaders/cubemap.vert",
                            "shaders/prefilter-map.frag"};

    Shader brdfLUTShader {"shaders/brdf.vert",
                          "shaders/brdf.frag"};

    Texture texture {"resources/skyboxes/GrandCanyon.hdr", true};

    unsigned int envCubemap = 0;
    IBL ibl;
};


#endif //RENDER_ENGINE_SKYBOX_HPP
