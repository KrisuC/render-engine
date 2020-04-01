//
// Created by Krisu on 2020/3/13.
//

#include "DirectionalShadow.hpp"
#include "WindowManager.hpp"
#include "StaticMesh.hpp"
#include "Scene.hpp"
#include "Transform.hpp"

float DirectionalShadow::size_of_ortho = 250.f;
float DirectionalShadow::depth = 1000.f;

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
    glBindTexture(GL_TEXTURE_2D, 0);

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
    static Shader shadowGenShader {"shader/Shadow-mapping/directional-Shadow.vert",
                                   "shader/Shadow-mapping/directional-Shadow.frag"};
    glm::mat4 lightProjection = glm::ortho<float>(
            -size_of_ortho, size_of_ortho, -size_of_ortho, size_of_ortho,
            10, depth
    );
    static const glm::vec3 worldUp {0, 1, 0};

    glm::vec3 front = glm::normalize(direction);
    glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
    glm::vec3 up    = glm::normalize(glm::cross(right, front));

    glm::mat4 lightView = glm::lookAt(position, position + front, up);

    this->lightSpaceTransform = lightProjection * lightView;
    shadowGenShader.UseShaderProgram();
    shadowGenShader.Set("lightSpaceTransform", lightSpaceTransform);
    /* Rendering scene at light's space */
    glCullFace(GL_FRONT); // fix peter panning
    auto& scene = Engine::GetInstance().GetCurrentScene();
    scene.RenderMeshes(shadowGenShader);
    glCullFace(GL_BACK);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Engine::GetInstance().GetRenderer().ResetViewport();
    //DEBUG_TEXTURE2D(depthMap);
}

DirectionalShadow::~DirectionalShadow() {
    glDeleteBuffers(1, &depthMapFBO);
    glDeleteTextures(1, &depthMap);
}
