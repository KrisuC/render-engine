//
// Created by Krisu on 2019-12-09.
//

#ifndef RENDER_ENGINE_SGNODE_HPP
#define RENDER_ENGINE_SGNODE_HPP

#include <vector>
#include "basic/Light.hpp"
#include "basic/Transform.hpp"
#include "basic/Object.hpp"

// A scene graph node
// TODO: add specific post-effects, but how?
class SGNode {
public:
    SGNode() = default;

    std::vector<Object> objects;

    std::vector<SGNode> childs;
    SGNode *parent;

    Transform localTranform;
    std::vector<Light> lights;
};


#endif //RENDER_ENGINE_SGNODE_HPP
