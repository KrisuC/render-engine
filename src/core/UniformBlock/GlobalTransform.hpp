//
// Created by Krisu on 2020/3/10.
//

#ifndef RENDER_ENGINE_GLOBALTRANSFORM_HPP
#define RENDER_ENGINE_GLOBALTRANSFORM_HPP

#include <glm/gtc/type_ptr.hpp>

#include "UniformBlock.hpp"


class GlobalTransform : public UniformBlock {
public:
    explicit GlobalTransform() :
            UniformBlock("GlobalTransform", 2 * sizeof(glm::mat4)) { }

    void UpdateProjeciton(glm::mat4 const& projection) {
        SetBufferSubData(0, sizeof(glm::mat4), glm::value_ptr(projection));
    }

    void UpdateView(glm::mat4 const& view) {
        SetBufferSubData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    }
};


#endif //RENDER_ENGINE_GLOBALTRANSFORM_HPP
