//
// Created by Krisu on 2019-12-06.
//

#ifndef RENDER_ENGINE_LIGHT_HPP
#define RENDER_ENGINE_LIGHT_HPP

#include <glm/glm.hpp>
#include <stdexcept>

#include "Component.hpp"

/*
 * Light is responsible for holding a specific Type of light
 * 3 types of lights are supported
 *
 * 1. Point light: position and color
 * 2. Directional light: direction and color
 * 3. Spot light: position, direction (of the cone center), color and cone angle
 *    measured in radian
 *
 * usage:
 * 1. create light using class interface (on stack)
 *
 * Light plight(LightType::Point);
 * plight.SetPosition(...);
 * plight.SetColor(...);
 *
 * 2. create light using helper function (on stack)
 *
 * auto position = {...};
 * auto color = {...};
 * Light plight = PointLight(position, color);
 *
 * Required: Transform
 */

enum class LightType {
    Directional = 0,
    Point = 1,
    Spot = 2
};

class Light : public Component {
    using vec3 = glm::vec3;

public:
    explicit Light(LightType lightType) : ltype(lightType) { }

    inline void SetConeAngleInRadian(float cone);

    inline void SetColor(float r, float g, float b);

    inline void SetCastShadows(bool cast);

    // deprecated:

public:
    LightType ltype;

    float cone_angle_in_radian {};
    glm::vec3 color{};
    bool castShadows = true;

    // deprecated:
    glm::vec3 position{};
    glm::vec3 direction{};

    friend class Shader;
};


void Light::SetConeAngleInRadian(float cone) {
    assert(ltype == LightType::Spot);
    cone_angle_in_radian = cone;
}

void Light::SetCastShadows(bool cast) {
    castShadows = cast;
}

void Light::SetColor(float r, float g, float b) {
    color = {r, g, b};
}


Light PointLight(glm::vec3 position, glm::vec3 color);

Light DirectionalLight(glm::vec3 direction, glm::vec3 color);

Light SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color,
                float cone_angle_in_radian);


#endif //RENDER_ENGINE_LIGHT_HPP
