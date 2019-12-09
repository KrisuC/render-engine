//
// Created by Krisu on 2019-12-09.
//

#ifndef RENDER_ENGINE_SGNODE_HPP
#define RENDER_ENGINE_SGNODE_HPP

#include <vector>
#include "basic/Light.hpp"
#include "basic/Transform.hpp"

// A scene graph node
class SGNode {
public:
    SGNode() = default;

private:
    Transform localTranform;
    std::vector<Light> lights;
};


#endif //RENDER_ENGINE_SGNODE_HPP
