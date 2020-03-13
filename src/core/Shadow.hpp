//
// Created by Krisu on 2020/3/13.
//

#ifndef RENDER_ENGINE_SHADOW_HPP
#define RENDER_ENGINE_SHADOW_HPP

#include "Engine.hpp"
#include "Renderer.hpp"

class Shadow {
public:
    explicit Shadow(const int shadow_map_w=1024, const int shadow_map_h=1024) {
        glGenFramebuffers(1, &depthMapFBO);

        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                shadow_map_w, shadow_map_h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Shadow map frame buffer not complete");
        }

        /*
         render to depth map
        glViewport(0, 0, shadow_map_w, shadow_map_h);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
         TODO: Render scene!
         RenderScene
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        auto [scr_w, scr_h] = Engine::GetInstance().GetRenderer().GetWindowSize();
        glViewport(0, 0, scr_w, scr_h);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         Render normal Scene
        */
    }

public:
    unsigned int depthMapFBO = 0;
    unsigned int depthMap = 0;
};


#endif //RENDER_ENGINE_SHADOW_HPP
