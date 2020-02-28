//
// Created by Krisu on 2019-12-09.
//

#ifndef RENDER_ENGINE_TRANSFORM_HPP
#define RENDER_ENGINE_TRANSFORM_HPP

#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
    Transform() : synthesis(glm::mat4(1.0f)) {}

    void Rotate(float axis_x, float axis_y, float axis_z, float angle_in_degree) {
        synthesis = glm::rotate(synthesis,
                glm::radians(angle_in_degree), {axis_x, axis_y, axis_z});
    }
    void Translate(float x, float y, float z) {
        synthesis = glm::translate(synthesis, {x, y, z});
    }
    void Scale(float x, float y, float z) {
        synthesis = glm::scale(synthesis, {x, y, z});
    }

private:
    glm::mat4 synthesis;

    friend class Shader;
};


#endif //RENDER_ENGINE_TRANSFORM_HPP
