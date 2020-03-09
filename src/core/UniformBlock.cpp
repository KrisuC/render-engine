//
// Created by Krisu on 2020/3/7.
//

#include "UniformBlock.hpp"
#include "Shader.hpp"

UniformBlock::UniformBlock(int bytes, int binding_point) {
    glGenBuffers(1, &ubo);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, bytes, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0); // unbind

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, bytes);
}

void UniformBlock::SetBufferSubData(int offset, int size, float *value) {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, value);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void
UniformBlock::BindShader(Shader const &shader, const char *uniform_block_name) {
    unsigned ub_index = glGetUniformBlockIndex(shader.id, uniform_block_name);
    glUniformBlockBinding(shader.id, ub_index, 0);
}
