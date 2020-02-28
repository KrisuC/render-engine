//
// Created by Krisu on 2019-12-06.
//

#include "Light.hpp"

Light PointLight(glm::vec3 position, glm::vec3 color) {
    Light light(LightType::Point);
    light.SetPosition(position);
    light.SetColor(color);
    return light;
}

Light DirectionalLight(glm::vec3 direction, glm::vec3 color) {
    Light light(LightType::Directional);
    light.SetDirection(direction);
    light.SetColor(color);
    return light;
}

Light SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color,
                float cone_angle_in_radian) {
    Light light(LightType::Spot);
    light.SetPosition(position);
    light.SetDirection(direction);
    light.SetColor(color);
    light.SetConeAngleInRadian(cone_angle_in_radian);
    return light;
}
