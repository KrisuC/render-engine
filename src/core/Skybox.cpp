#include <utility>

//
// Created by Krisu on 2019-12-06.
//

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Skybox.hpp"
#include "Texture.hpp"
#include "StaticMesh.hpp"
#include "Scene.hpp"
#include "Engine.hpp"


void Skybox::Render() {
    renderShader.UseShaderProgram();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

    auto& camera = Engine::GetInstance().GetCurrentScene().GetCurrentCamera();
    auto [w, h] = Engine::GetInstance().GetRenderer().GetWindowSize();
    glm::mat4 projection = glm::perspective(glm::radians(camera.GetFovy()),
                                            static_cast<float>(w)/h, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    renderShader.Set("view", view);
    renderShader.Set("projection", projection);

    BuiltinMesh::RenderCube();
}

Skybox::Skybox(Texture const& skyboxTexture) : texture(skyboxTexture)  {
    unsigned int irradianceMapID = 0;
    unsigned int prefilterMapID = 0;
    unsigned int brdfLUTTextureID = 0;

    // convert equirectangular to cubemap
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glDepthFunc(GL_LEQUAL);

    unsigned int captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    unsigned int dim = 4096;

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, dim, dim);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, captureRBO);
    // generate corresponding cubemap
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                     dim, dim, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // generate mipmaps
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glm::mat4 captureProjection
            = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 10.f);
    glm::vec3 orig = {0, 0, 0};
    glm::mat4 captureViews[] = {
            // eye, center, up
            glm::lookAt(orig, glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(orig, glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(orig, glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(orig, glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(orig, glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(orig, glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
    equirectToCubemapShader.UseShaderProgram();
    equirectToCubemapShader.Set("equirectangularMap", 0);
    equirectToCubemapShader.Set("projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture.ID());

    glViewport(0, 0, dim, dim);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (int i = 0; i < 6; i++) {
        equirectToCubemapShader.Set("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        BuiltinMesh::RenderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // -------------------------------------------------
    // Generating maps for IBL
    {
        // generate irradiance map
        glGenTextures(1, &irradianceMapID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMapID);
        for (int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32,
                         32,
                         0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                        GL_CLAMP_TO_EDGE);
        // linear filter will handle the low resolution texture
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

        irradianceShader.UseShaderProgram();
        irradianceShader.Set("environmentMap", 0);
        irradianceShader.Set("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glViewport(0, 0, 32, 32);
        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        {
            for (int i = 0; i < 6; i++) {
                irradianceShader.Set("view", captureViews[i]);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                       GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                                       irradianceMapID, 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                BuiltinMesh::RenderCube();
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //--------------------------------------------------------------------
        // generate prefilter map for skybox specular lighting
        int res_prfmap = 128;
        glGenTextures(1, &prefilterMapID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilterMapID);
        for (int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                         res_prfmap, res_prfmap, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        prefilterShader.UseShaderProgram();
        prefilterShader.Set("environmentMap", 0);
        prefilterShader.Set("projection", captureProjection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        {
            unsigned int maxMipLevels = 5;
            for (int mip = 0; mip < maxMipLevels; mip++) {
                // resize framebuffer according to mip-level
                unsigned int mipWidth = 128 * std::pow(0.5, mip);
                unsigned int mipHeight = 128 * std::pow(0.5, mip);
                glBindRenderbuffer(GL_RENDERBUFFER, captureFBO);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                                      mipWidth, mipHeight);
                glViewport(0, 0, mipWidth, mipHeight);

                float roughness = (float) mip / (float) (maxMipLevels - 1);
                prefilterShader.Set("roughness", roughness);
                for (int i = 0; i < 6; i++) {
                    prefilterShader.Set("view", captureViews[i]);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                           GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                                           prefilterMapID, mip);

                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    BuiltinMesh::RenderCube();
                }
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //--------------------------------------------------------------------
        // brdf precomte
        glGenTextures(1, &brdfLUTTextureID);

        glBindTexture(GL_TEXTURE_2D, brdfLUTTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 512, 512, 0, GL_RG, GL_FLOAT,
                     0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
        {
            glBindRenderbuffer(GL_RENDERBUFFER, captureFBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512,
                                  512);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                   GL_TEXTURE_2D, brdfLUTTextureID, 0);

            glViewport(0, 0, 512, 512);
            brdfLUTShader.UseShaderProgram();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            BuiltinMesh::RenderQuad();
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    Texture irradiance(irradianceMapID, TextureType::CubeMap);
    Texture prefilter(prefilterMapID, TextureType::CubeMap);
    Texture brdfLUTTexture(brdfLUTTextureID, TextureType::Texture2D);
    ibl = std::move(IBL(std::move(irradiance),
            std::move(prefilter), std::move(brdfLUTTexture)));
}

Skybox::Skybox() :
Skybox(Texture("asset/texture/skybox/GrandCanyon.hdr") ) {}



