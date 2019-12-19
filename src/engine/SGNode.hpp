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

    inline void setLocalTransform(const Transform &t) { localTransform = t; }
    inline void appendObjects(Object *objp) { objects.push_back(objp); }
    inline void appendNodes(SGNode *nodep) {
        nodep->parent = this;
        childNodes.push_back(nodep);
    }

    template <typename ...Args>
    void setGlobalShaderValue(const std::string &name, Args &&...args) {
        for (auto obj : objects) {
            obj->setShaderUnif(name, std::forward<Args>(args)...);
        }
        for (auto child : childNodes) {
            child->setGlobalShaderValue(name, std::forward<Args>(args)...);
        }
    }

    void updateLights() {
        for (int i = 0; i < lights.size(); i++) {
            this->setGlobalShaderValue(
                    "lightPositions[" + std::to_string(i) + "]",
                    lights[i].position);
            this->setGlobalShaderValue(
                    "lightColors[" + std::to_string(i) + "]",
                    lights[i].color);
        }
    }

    inline void setLights(const std::vector<Light> &lights) {
        this->lights = lights;
    }

private:
    std::vector<Object *> objects;

    std::vector<SGNode *> childNodes;
    SGNode *parent = nullptr;

    // If it's root, it may contain view, projection matrix?
    Transform localTransform;
    std::vector<Light> lights;
};


#endif //RENDER_ENGINE_SGNODE_HPP
