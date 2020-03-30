//
// Created by Krisu on 2020/3/13.
//

#ifndef RENDER_ENGINE_POINTSHADOW_HPP
#define RENDER_ENGINE_POINTSHADOW_HPP

#include <array>

#include <glm/gtc/matrix_transform.hpp>

#include "Shadow.hpp"
#include "WindowManager.hpp"
#include "Shader.hpp"
#include "Scene.hpp"
#include "Engine.hpp"
#include "StaticMesh.hpp"
#include "Transform.hpp"


class PointShadow : public Shadow {
public:
    explicit PointShadow(int map_width=1024, int map_height=1024);

    void GenerateShadowMap(const glm::vec3 &position, const glm::vec3 &direction,
                           float cone_in_degree) override;

    ~PointShadow();

public:
    unsigned depthMapFBO = 0;
    unsigned depthCubemap = 0;

    std::array<glm::mat4, 6> shadowTransformsPV;

    const int width, height;
};


#endif //RENDER_ENGINE_POINTSHADOW_HPP
