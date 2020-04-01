//
// Created by Krisu on 2020/3/13.
//

#ifndef RENDER_ENGINE_SHADOW_HPP
#define RENDER_ENGINE_SHADOW_HPP

#include <glm/glm.hpp>

class Shadow {
public:
    virtual void GenerateShadowMap(const glm::vec3 &position, const glm::vec3 &direction,
                                   float cone_in_degree) = 0;
};


#endif //RENDER_ENGINE_SHADOW_HPP
