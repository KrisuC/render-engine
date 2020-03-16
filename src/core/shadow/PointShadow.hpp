//
// Created by Krisu on 2020/3/13.
//

#ifndef RENDER_ENGINE_POINTSHADOW_HPP
#define RENDER_ENGINE_POINTSHADOW_HPP

#include <array>

#include <glm/gtc/matrix_transform.hpp>

#include "Shadow.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"


class PointShadow : public Shadow {
public:
    explicit PointShadow(int map_width=1024, int map_height=1024) :
            width(map_width), height(map_height) {

        glGenTextures(1, &depthCubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
        for (int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                    width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void GenerateShadowMap(glm::vec3 position, glm::vec3 direction,
                           float cone_in_degree) override {
        glViewport(0, 0, width, height);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        // Configure shader and Matrices
        float aspect = static_cast<float>(width) / height;
        float near = 1.0f, far = 50.f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.f),
                aspect, near, far);
        static const glm::vec3 z {0, 0, 1},
                               y {0, 1, 0},
                               x { 1, 0, 0 };

        static std::array<glm::mat4, 6> shadowViews {
                glm::lookAt(position, position +  x, -y),
                glm::lookAt(position, position + -x, -y),
                glm::lookAt(position, position +  y,  z),
                glm::lookAt(position, position + -y,  z),
                glm::lookAt(position, position +  z, -x),
                glm::lookAt(position, position + -z, -x)
        };
        static const std::string dir("shader/shadow-mapping/point-shadow");
//        static Shader shadowGenShader {dir + ".vert", dir + ".frag", dir + ".geom" };
        // Render shadow cube map scene
//        glViewport(0, 0, )
    }

private:
    unsigned depthMapFBO = 0;
    unsigned depthCubemap = 0;

    const int width, height;
};


#endif //RENDER_ENGINE_POINTSHADOW_HPP
