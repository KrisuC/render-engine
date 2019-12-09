//
// Created by Krisu on 2019-12-09.
//

#ifndef RENDER_ENGINE_TRANSFORM_HPP
#define RENDER_ENGINE_TRANSFORM_HPP

#include <glm/gtc/matrix_transform.hpp>

// A transform is append to a node/objects in scene graph, to perform a local
// transformation according to the local space of its parent node
class Transform {
public:
    Transform(): mat(glm::identity<glm::mat4>()) {}
    explicit Transform(const glm::mat4 &mat) : mat(mat) {}
    Transform(const Transform &) = default;
    Transform &operator=(const glm::mat4 &rhs) { mat = rhs; return *this; }
    Transform &operator=(const Transform &) = default;

    const glm::mat4 &getMatrix() { return mat; }

private:
    glm::mat4 mat;
};


#endif //RENDER_ENGINE_TRANSFORM_HPP
