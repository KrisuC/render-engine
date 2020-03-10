//
// Created by Krisu on 2020/3/10.
//

#ifndef RENDER_ENGINE_LIGHTINFORMATION_HPP
#define RENDER_ENGINE_LIGHTINFORMATION_HPP

#include <glm/gtc/type_ptr.hpp>

#include "UniformBlock.hpp"
#include "Light.hpp"

class LightInformation : public UniformBlock {
public:

    explicit LightInformation() :
            UniformBlock("LightInformation", 1024) {}

    void UpdateLightSize(int size) {
        SetBufferSubData(243*N, N, &size);
    }
    void UpdateCameraPosition(glm::vec3 const& camera_pos) {
        SetBufferSubData(240*N, 3*N, value_ptr(camera_pos));
    }
    void UpdateLight(int index, Light const& light) {
        using glm::value_ptr;
        const int elem_offset = 240 * N * index;
        SetBufferSubData(elem_offset + 0, 3 * N, value_ptr(light.position));
        SetBufferSubData(elem_offset + 3 * N,  N, &light.cone_angle_in_radian);

        SetBufferSubData(elem_offset + 4 * N, 3 * N, value_ptr(light.direction));
        SetBufferSubData(elem_offset + 7 * N, N, &light.ltype);

        SetBufferSubData(elem_offset + 8 * N, 3 * N, value_ptr(light.color));
    }

    static constexpr const int N = 4;
};


#endif //RENDER_ENGINE_LIGHTINFORMATION_HPP
