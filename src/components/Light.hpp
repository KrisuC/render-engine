//
// Created by Krisu on 2019-12-06.
//

#ifndef RENDER_ENGINE_LIGHT_HPP
#define RENDER_ENGINE_LIGHT_HPP


#include <stdexcept>
#include <utility>

#include <glm/glm.hpp>

#include "DirectionalShadow.hpp"
#include "PointShadow.hpp"
#include "SpotShadow.hpp"
#include "Transform.hpp"
#include "Material.hpp"
#include "Shader.hpp"
#include "Engine.hpp"
#include "Scene.hpp"
#include "Debug.hpp"

/*
 * Light, yes, light.
 */

enum class LightType {
    Directional = 0,
    Point = 1,
    Spot = 2
};


/* LightType::Point -> PointShadow etc... */
template <LightType lightType>
using ShadowGen =
        std::conditional_t<lightType==LightType::Point,
            PointShadow,
            std::conditional_t<lightType==LightType::Directional,
                DirectionalShadow,
                SpotShadow>
>;


class LightManager {
    static int total_cnt;
    template <LightType lightType,
             typename ShadowMapGenerator> friend class Light;

public:
    static int GetLightsCount() { return total_cnt; }
};


template <LightType lightType,
          typename ShadowMapGenerator = ShadowGen<lightType>>
class Light : public Component {
    static_assert(std::is_base_of_v<Shadow, ShadowMapGenerator>,
                  "Error: ShadowMapGenerator must be derived class of Shadow");

public:

    void BeforeRenderPass() override;

public:
    Light();

    void SetConeAngleInRadian(float cone);;

    void SetColor(float r, float g, float b);
    void SetColor(glm::vec3 color_);

    void SetCastShadows(bool cast);

    void GenerateShadow(glm::vec3 position, glm::vec3 direction,
                        float cone_in_degree);

private:
    float cone_angle_in_radian { glm::radians(45.f) };

    glm::vec3 color {1.0f };

    bool cast_shadows = true;

    ShadowMapGenerator shadow {};

    int index = -1;

    friend ShadowMapGenerator;

public:
    // TODO: fix these with Transform
    glm::vec3 position {};
    glm::vec3 direction {};
};



template<LightType lightType, typename ShadowMapGenerator>
void Light<lightType, ShadowMapGenerator>::GenerateShadow(glm::vec3 position,
                                                          glm::vec3 direction,
                                                          float cone_in_degree) {
    shadow.GenerateShadowMap(position, direction, cone_in_degree);
}


template<LightType lightType, typename ShadowMapGenerator>
Light<lightType, ShadowMapGenerator>::Light() {
    this->index = LightManager::total_cnt;
    LightManager::total_cnt++;
}


template<LightType lightType, typename ShadowMapGenerator>
void Light<lightType, ShadowMapGenerator>::BeforeRenderPass() {
    this->GenerateShadow(position, direction, cone_angle_in_radian);

    /* Updating shadow maps -- should be done for all object */
    for (auto& up_game_obj : Engine::GetInstance().GetCurrentScene().GetListOfObeject()) {
        Shader& shader = up_game_obj->GetComponent<Material>().GetShader();
        shader.UseShaderProgram();
        shader.Set("lightSpaceTransform", shadow.lightSpaceTransform);
        int unit = 14;
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, shadow.depthMap);
        shader.Set("shadowMap", unit);
    }

    /* Updating LightInformation Uniform Block */
    auto& lightInfo = Engine::GetInstance().GetUniformBuffer<LightInformation>();
    lightInfo.UpdateLightColor(index, color);
    lightInfo.UpdateLightCone(index, cone_angle_in_radian);
    lightInfo.UpdateLightDirection(index, direction);
    lightInfo.UpdateLightType(index, static_cast<int>(lightType));
    lightInfo.UpdateLightPosition(index, position);
}


template<LightType lightType, typename ShadowMapGenerator>
void Light<lightType, ShadowMapGenerator>::SetConeAngleInRadian(float cone) {
    static_assert(lightType == LightType::Spot, "Only Spot light has cone to set");
    cone_angle_in_radian = cone;
}


template<LightType lightType, typename ShadowMapGenerator>
void Light<lightType, ShadowMapGenerator>::SetColor(glm::vec3 color_) { color = color_; }


template<LightType lightType, typename ShadowMapGenerator>
void Light<lightType, ShadowMapGenerator>::SetColor(float r, float g, float b) { color = {r, g, b}; }


template<LightType lightType, typename ShadowMapGenerator>
void Light<lightType, ShadowMapGenerator>::SetCastShadows(bool cast) { cast_shadows = cast; }


using PointLight      = Light<LightType::Point>;
using SpotLight       = Light<LightType::Spot>;
using DirectionalLight = Light<LightType::Directional>;


#endif //RENDER_ENGINE_LIGHT_HPP
