//
// Created by Krisu on 2019-12-03.
//

#ifndef RENDER_ENGINE_SCENEGRAPH_HPP
#define RENDER_ENGINE_SCENEGRAPH_HPP

#include "basic/Object.hpp"
#include "basic/Transform.hpp"
#include <vector>
#include <memory>

// TODO: A real Scene Graph
class SceneGraph : public Object {
public:
    SceneGraph() = default;
    inline void appendObject(Object *obj) {
        scene.push_back(std::make_shared<Object>(obj));
    }
    void prepare() override {
        for (auto &objp : scene) {
            objp->prepare();
        }
    }
    void render() override {
        for (auto &objp : scene) {
            objp->render();
        }
    }

private:
    // A linear structure for now...
    std::vector<std::shared_ptr<Object>> scene;
    Transform view;
    Transform projection;
};


#endif //RENDER_ENGINE_SCENEGRAPH_HPP
