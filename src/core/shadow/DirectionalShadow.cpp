//
// Created by Krisu on 2020/3/13.
//

#include "DirectionalShadow.hpp"
#include "WindowManager.hpp"
#include "Mesh.hpp"
#include "Scene.hpp"
#include "Transform.hpp"

DirectionalShadow::DirectionalShadow(const int map_width, const int map_height)
        :
        width(map_width), height(map_height) {

    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
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
}

void
DirectionalShadow::GenerateShadowMap(const glm::vec3 &position, const glm::vec3 &direction,
                                     float cone_in_degree) {
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glViewport(0, 0, width, height);

    glClear(GL_DEPTH_BUFFER_BIT);
    static Shader shadowGenShader {"shader/shadow-mapping/directional-shadow.vert",
                                   "shader/shadow-mapping/directional-shadow.frag"};
    glm::mat4 lightProjection = glm::ortho<float>(
            -10, 10, -10, 10,
            1.0, shadow_depth
    );
    static const glm::vec3 global_up {0, 1, 0};
    glm::vec3 right = glm::cross(direction, global_up);
    glm::vec3 up = glm::cross(right, direction);
    glm::mat4 lightView = glm::lookAt(position, direction, up);

    this->lightSpaceTransform = lightProjection * lightView;
    shadowGenShader.UseShaderProgram();
    shadowGenShader.Set("lightSpaceTransform", lightSpaceTransform);
    glCullFace(GL_FRONT); // fix peter panning
    /* Rendering scene at light's space */
    auto& scene = Engine::GetInstance().GetCurrentScene();
    for (auto& up_game_obj : scene.GetListOfObeject()) {
        try {
            auto& mesh = up_game_obj->GetComponent<Mesh>();
            auto& transform = up_game_obj->GetComponent<Transform>();
            shadowGenShader.Set("model", transform.GetMatrix());
            mesh.DrawCall();
        } catch (NoComponent &) {
            continue;
        }
    }
    glCullFace(GL_BACK);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Engine::GetInstance().GetRenderer().ResetViewport();
}

DirectionalShadow::~DirectionalShadow() {
    glDeleteBuffers(1, &depthMapFBO);
    glDeleteTextures(1, &depthMap);
}
