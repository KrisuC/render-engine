//
// Created by Krisu on 2020/3/10.
//

#ifndef RENDER_ENGINE_GLOBALTRANSFORMATION_HPP
#define RENDER_ENGINE_GLOBALTRANSFORMATION_HPP

#include <glm/gtc/type_ptr.hpp>

#include "UniformBlock.hpp"


class GlobalTransformation : public UniformBlock {
public:
    explicit GlobalTransformation() :
            UniformBlock("GlobalTransformation", 2 * sizeof(glm::mat4)) { }

    void UpdateView(glm::mat4 const& view) {
        SetBufferSubData(0, sizeof(glm::mat4), glm::value_ptr(view));
    }

    void UpdateProjeciton(glm::mat4 const& projection) {
        SetBufferSubData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
    }
};


#endif //RENDER_ENGINE_GLOBALTRANSFORMATION_HPP
