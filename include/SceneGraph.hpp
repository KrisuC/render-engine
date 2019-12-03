//
// Created by Krisu on 2019-12-03.
//

#ifndef RENDER_ENGINE_SCENEGRAPH_HPP
#define RENDER_ENGINE_SCENEGRAPH_HPP

#include <Object.hpp>
#include <vector>
#include <memory>

// TODO: A better scene graph
class SceneGraph : public Object {
public:
    SceneGraph() = default;
    inline void appendObject(std::unique_ptr<Object> obj) { scene.push_back(obj); }
    void prepare() override {
        for (std::unique_ptr<Object> &objp : scene) {
            objp->prepare();
        }
    }
    void render() override {
        for (std::unique_ptr<Object> &objp : scene) {
            objp->render();
        }
    }


private:
    std::vector<std::unique_ptr<Object>> scene;
};


#endif //RENDER_ENGINE_SCENEGRAPH_HPP
