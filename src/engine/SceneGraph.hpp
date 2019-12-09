//
// Created by Krisu on 2019-12-03.
//

#ifndef RENDER_ENGINE_SCENEGRAPH_HPP
#define RENDER_ENGINE_SCENEGRAPH_HPP

#include <vector>
#include <memory>

#include "basic/Object.hpp"
#include "basic/Transform.hpp"
#include "engine/SGNode.hpp"

// SceneGraph -> Node -> Node -> Object
//            -> Node -> ...
//            -> ...
//
// SceneGraph       : entry of the graph
// SGNode(interior) : responsible for lights and other special effects
// Obeject(leaf)    : real entity to be rendered

class SceneGraph {
public:
    SceneGraph() = default;
    inline void appendObject(Object *obj) {
        scene.push_back(std::make_shared<Object>(obj));
    }
    void prepareObjects() {
        for (auto &objp : scene) {
            objp->setShaderUnif("projection", projection);
            objp->prepare();
        }
    }
    void renderObjects() {
        for (auto &objp : scene) {
            objp->render();
        }
    }
    void updateView() {
        for (auto &objp : scene) {
            objp->setShaderUnif("view", view);
        }
    }

private:
    // TODO: replace with a tree-like structure
    std::vector<std::shared_ptr<Object>> scene;

    SGNode *root;

    Transform view;
    Transform projection;
};


#endif //RENDER_ENGINE_SCENEGRAPH_HPP
