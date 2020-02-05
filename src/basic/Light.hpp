//
// Created by Krisu on 2019-12-06.
//

#ifndef RENDER_ENGINE_LIGHT_HPP
#define RENDER_ENGINE_LIGHT_HPP

#include <glm/glm.hpp>
#include <stdexcept>

enum LightType { Directional, Point, Spot };

struct DirectionalLight {
    using vec3 = glm::vec3;

    vec3 direction;
    vec3 color;
};

struct SpotLight {
    using vec3 = glm::vec3;

    vec3 position;
    vec3 direciton;
    vec3 color;
    float cone_angle_in_radian;
};

struct PointLight {
    using vec3 = glm::vec3;

    vec3 position;
    vec3 color;
};

/*
 * Light is responsible for holding a specific type of light
 */

class Light {
    using vec3 = glm::vec3;

public:
    explicit Light(LightType lightType) : lightType(lightType) {
        if (lightType == LightType::Directional) {
            lightPtr = new DirectionalLight;
        } else if (lightType == LightType::Spot) {
            lightPtr = new SpotLight;
        } else if (lightType == LightType::Point) {
            lightPtr = new PointLight;
        } else {
            throw std::runtime_error("unknown LightType");
        }
    }
    ~Light() {
        if (lightType == LightType::Directional) {
            delete (DirectionalLight*)lightPtr;
        } else if (lightType == LightType::Spot) {
            delete (SpotLight*)lightPtr;
        } else if (lightType == LightType::Point) {
            delete (PointLight*)lightPtr;
        }
    }

    void initDirectionalLight(vec3 direction, vec3 color) {
        assert(lightType == LightType::Directional);
        *static_cast<DirectionalLight *>(lightPtr) = {direction, color};
    }

    void initSpotLight(vec3 position, vec3 direction, vec3 color,
                       float cone_angle_in_radian) {
        assert(lightType == LightType::Spot);
        *static_cast<SpotLight *>(lightPtr) = {
                position, direction, color, cone_angle_in_radian
        };
    }

    void initPointLight(vec3 position, vec3 color) {
        assert(lightType == LightType::Point);
        *static_cast<PointLight *>(lightPtr) = { position, color };
    }

private:
    void *lightPtr;
    LightType lightType;
};


#endif //RENDER_ENGINE_LIGHT_HPP
