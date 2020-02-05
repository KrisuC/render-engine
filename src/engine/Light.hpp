//
// Created by Krisu on 2019-12-06.
//

#ifndef RENDER_ENGINE_LIGHT_HPP
#define RENDER_ENGINE_LIGHT_HPP

#include <glm/glm.hpp>
#include <stdexcept>

/*
 * Light is responsible for holding a specific type of light
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
 * plight.setPosition(...);
 * plight.setColor(...);
 *
 * 2. create light using helper function (on stack)
 *
 * auto position = {...};
 * auto color = {...};
 * Light plight = PointLight(position, color);
 *
 */

enum LightType { Directional, Point, Spot };

class Light {
    using vec3 = glm::vec3;

public:
    explicit Light(LightType lightType) : ltype(lightType) { }

    inline void setPosition(vec3 pos) {
        assert(ltype == LightType::Point || ltype == LightType::Spot);
        position = pos;
    }

    inline void setDirection(vec3 dir) {
        assert(ltype == LightType::Directional || ltype == LightType::Spot);
        direciton = dir;
    }

    inline void setColor(vec3 c) {
        color = c;
    }

    inline void setConeAngleInRadian(float cone) {
        assert(ltype == LightType::Spot);
        cone_angle_in_radian = cone;
    }

private:
    LightType ltype;

    vec3 position;
    vec3 direciton;
    vec3 color;

    float cone_angle_in_radian;
};

Light PointLight(glm::vec3 position, glm::vec3 color) {
    Light light(LightType::Point);
    light.setPosition(position);
    light.setColor(color);
    return light;
}

Light DirectionalLight(glm::vec3 direction, glm::vec3 color) {
    Light light(LightType::Directional);
    light.setDirection(direction);
    light.setColor(color);
    return light;
}

Light SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color,
                float cone_angle_in_radian) {
    Light light(LightType::Spot);
    light.setPosition(position);
    light.setDirection(direction);
    light.setColor(color);
    light.setConeAngleInRadian(cone_angle_in_radian);
}


#endif //RENDER_ENGINE_LIGHT_HPP
