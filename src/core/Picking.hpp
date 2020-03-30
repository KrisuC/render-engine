//
// Created by Krisu on 2020/3/27.
//

#ifndef RENDER_ENGINE_PICKING_HPP
#define RENDER_ENGINE_PICKING_HPP

#include "WindowManager.hpp"

class Picking {
public:
    Picking(unsigned window_width, unsigned window_height) {
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // Texture for primitive information
        glGenTextures(1, &picking_texture);
        glBindTexture(GL_TEXTURE_2D, picking_texture);
        glTexImage2D(GL_TEXTURE_2D,
                0, /* level */
                GL_RGB32F, /* internal format */
                window_width, window_height,
                0, /* border */
                GL_RGB, /* intial fortmat */
                GL_FLOAT, /* type */
                nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0, /* attachment */
                GL_TEXTURE_2D, /* texture target */
                picking_texture, 0);

        // Texture for depth buffer
    }

    void EnableWriting();

    void DisableWriting();

    ~Picking() {

    }

private:
    unsigned fbo;
    unsigned picking_texture;
    unsigned depth_texture;
};


#endif //RENDER_ENGINE_PICKING_HPP
