//
// Created by Krisu on 2020/3/13.
//

#include "PointShadow.hpp"
#include "MeshRenderer.hpp"


PointShadow::PointShadow(int map_width, int map_height) :
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


void PointShadow::GenerateShadowMap(const glm::vec3 &position,
                                    const glm::vec3 &direction,
                                    float cone_in_degree) {
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    /* Configure shader and Matrices */
    float aspect = static_cast<float>(width) / height;
    float near = 1.0f, far = shadow_depth;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.f),
                                            aspect, near, far);
    static const glm::vec3 Z {0, 0, 1},
            Y {0, 1, 0},
            X {1, 0, 0 };

    std::array<glm::mat4, 6> shadowViews {
            glm::lookAt(position, position +  X, -Y),
            glm::lookAt(position, position + -X, -Y),
            glm::lookAt(position, position +  Y,  Z),
            glm::lookAt(position, position + -Y,  Z),
            glm::lookAt(position, position +  Z, -X),
            glm::lookAt(position, position + -Z, -X)
    };
    static const std::string dir("shader/Shadow-mapping/point-Shadow");
    static Shader shadowGenShader {dir + ".vert", dir + ".frag", dir + ".geom" };
    for (int i = 0; i < shadowTransformsPV.size(); i++) {
        shadowTransformsPV[i] = shadowProj * shadowViews[i];
        shadowGenShader.Set("shadowTransformPV[" + std::to_string(i) + "]",
                            shadowTransformsPV[i]);
    }
    shadowGenShader.UseShaderProgram();
    shadowGenShader.Set("lightPos", position);
    shadowGenShader.Set("far_plane", far);
    /* Rendering scene to Shadow map */
    glCullFace(GL_FRONT); // fix peter panning
    auto& scene = Engine::GetInstance().GetCurrentScene();
    scene.RenderMeshes(shadowGenShader);
    glCullFace(GL_BACK);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Engine::GetInstance().GetRenderer().ResetViewport();
}

PointShadow::~PointShadow() {
    glDeleteFramebuffers(1, &depthMapFBO);
    glDeleteTextures(1, &depthCubemap);
}

