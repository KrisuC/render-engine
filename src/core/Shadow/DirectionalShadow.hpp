//
// Created by Krisu on 2020/3/13.
//

#ifndef RENDER_ENGINE_DIRECTIONALSHADOW_HPP
#define RENDER_ENGINE_DIRECTIONALSHADOW_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Light.hpp>

#include "Shadow.hpp"

class DirectionalShadow : public Shadow {
public:
    explicit DirectionalShadow(int map_width=10240, int map_height=10240);

    void GenerateShadowMap(const glm::vec3 &position, const glm::vec3 &direction,
                           float cone_in_degree) override;

    ~DirectionalShadow();

public:
    unsigned depthMapFBO = 0;
    unsigned depthMap = 0;
    const int width, height;

    glm::mat4 lightSpaceTransform;
    static float size_of_ortho;
    static float depth;
};


#endif //RENDER_ENGINE_DIRECTIONALSHADOW_HPP
