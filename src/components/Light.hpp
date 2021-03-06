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
#include "Shader.hpp"
#include "Engine.hpp"
#include "Scene.hpp"
#include "Debug.hpp"
#include "MeshRenderer.hpp"

/*
 * Light, yes, light.
 */

enum class LightType {
    Directional = 0,
    Point = 1,
    Spot = 2
};

class LightManager {
    static int total_cnt;
    template <LightType lightType, typename ShadowMapGenerator> friend class Light;

public:
    static int GetLightsCount() { return total_cnt; }
};


/************************************************************/

template <LightType lightType, typename ShadowMapGenerator>
class Light : public Component {
    static_assert(std::is_base_of_v<Shadow, ShadowMapGenerator>,
                  "Error: ShadowMapGenerator must be derived class of Shadow");
public:
    void Update() override {
        this->GenerateShadow(position, direction, cone_angle_in_radian);

        // TODO: support multiple shadow
        /* Updating Shadow maps -- should be done for all object */
        for (auto& pGameObject : Engine::GetInstance().GetCurrentScene().GetListOfObeject()) {
            Shader& shader = pGameObject->GetComponent<MeshRenderer>().material.GetShader();
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

public:
    Light() {
        DEBUG_LOG("Creating Light:", LightManager::total_cnt);
        this->index = LightManager::total_cnt;
        LightManager::total_cnt++;
    }

    void SetConeAngleInRadian(float cone) {
        static_assert(lightType == LightType::Spot, "Only Spot light has cone to set");
        cone_angle_in_radian = cone;
    }

    void SetColor(float r, float g, float b) { color = { r, g, b }; }

    void SetColor(glm::vec3 color_) { color = color_; }

    void SetCastShadows(bool cast) { cast_shadows = cast; }

    void GenerateShadow(glm::vec3 position, glm::vec3 direction,
                        float cone_in_degree) {
        if (!cast_shadows) { return; }
        shadow.GenerateShadowMap(position, direction, cone_in_degree);
    }

private:
    float cone_angle_in_radian { glm::radians(45.f) };

    glm::vec3 color { 1.0f };

    bool cast_shadows = true;

    ShadowMapGenerator shadow {};

    int index = -1;

    friend ShadowMapGenerator;

public:
    // TODO: fix these with Transform
    glm::vec3 position {};
    glm::vec3 direction {};
};


using PointLight      = Light<LightType::Point, PointShadow>;
using SpotLight       = Light<LightType::Spot, PointShadow>; // TODO: spot light shadow
using DirectionalLight = Light<LightType::Directional, DirectionalShadow>;


#endif //RENDER_ENGINE_LIGHT_HPP
