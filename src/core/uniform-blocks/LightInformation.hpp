//
// Created by Krisu on 2020/3/10.
//

#ifndef RENDER_ENGINE_LIGHTINFORMATION_HPP
#define RENDER_ENGINE_LIGHTINFORMATION_HPP

#include <glm/gtc/type_ptr.hpp>

#include "UniformBlock.hpp"


class LightInformation : public UniformBlock {
public:

    explicit LightInformation() :
            UniformBlock("LightInformation", 1024) {}

    /* for all lights */
    void UpdateLightSize(int size) {
        SetBufferSubData(243*N, N, &size);
    }
    void UpdateCameraPosition(glm::vec3 const& camera_pos) {
        SetBufferSubData(240*N, 3*N, value_ptr(camera_pos));
    }

    /* for single light */
    void UpdateLightPosition(int index, glm::vec3 position) {
        const int elem_offset = STRUCT_ELEM_SIZE * N * index;
        SetBufferSubData(elem_offset + 0, 3 * N, glm::value_ptr(position));
    }

    void UpdateLightDirection(int index, glm::vec3 direction) {
        const int elem_offset = STRUCT_ELEM_SIZE * N * index;
        SetBufferSubData(elem_offset + 4 * N, 3 * N, glm::value_ptr(direction));
    }

    void UpdateLightCone(int index, float cone_angle_in_radian) {
        const int elem_offset = STRUCT_ELEM_SIZE * N * index;
        SetBufferSubData(elem_offset + 3 * N,  N, &cone_angle_in_radian);
    }

    void UpdateLightType(int index, int /* Light Type */ ltype) {
        const int elem_offset = STRUCT_ELEM_SIZE * N * index;
        SetBufferSubData(elem_offset + 7 * N, N, &ltype);
    }

    void UpdateLightColor(int index, glm::vec3 color) {
        const int elem_offset = STRUCT_ELEM_SIZE * N * index;
        SetBufferSubData(elem_offset + 8 * N, 3 * N, glm::value_ptr(color));
    }

    static constexpr const int N = 4;
    static constexpr const int STRUCT_ELEM_SIZE = 240;
};


#endif //RENDER_ENGINE_LIGHTINFORMATION_HPP
