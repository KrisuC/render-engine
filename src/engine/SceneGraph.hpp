//
// Created by Krisu on 2019-12-03.
//

#ifndef RENDER_ENGINE_SCENEGRAPH_HPP
#define RENDER_ENGINE_SCENEGRAPH_HPP

#include <vector>
#include <memory>

#include "basic/Object.hpp"
#include "basic/Transform.hpp"
#include "basic/Camera.hpp"
#include "engine/SGNode.hpp"

// SceneGraph -> Node -> Node -> Object
//            -> Node -> ...
//            -> ...
//
// SceneGraph       : entry of the graph
// SGNode(interior) : responsible for lights and other special effects
// Obeject(leaf)    : real entity to be rendered

// TODO: A method to construct a scene graph from a scene description
//       file
class SceneGraph {
public:
    SceneGraph() = default;

    inline void setRoot(SGNode *root) { this->p_root = root; }
    inline SGNode *root() { return p_root; }

    void prepareScene() {
        prepareNode(p_root);
    }
    void renderScene() {
//        p_root->updateLights();
        renderNode(p_root);
    }

private:
    void renderNode(SGNode *p_node) {
        for (auto obj : p_node->objects) {
            obj->render();
        }
        for (auto child : p_node->childNodes) {
            renderNode(child);
        }
    }
    void prepareNode(SGNode *p_node) {
        for (auto obj : p_node->objects) {
            obj->prepare();
        }
        for (auto child : p_node->childNodes) {
            prepareNode(child);
        }
    }

private:
    SGNode *p_root;
};


#endif //RENDER_ENGINE_SCENEGRAPH_HPP
