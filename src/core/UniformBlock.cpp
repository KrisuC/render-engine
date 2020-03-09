//
// Created by Krisu on 2020/3/7.
//

#include "UniformBlock.hpp"

#include <utility>
#include "Shader.hpp"

UniformBlock::UniformBlock(int bytes, int binding_point,
                           std::string uniform_block_name)
        : uniformBlockName(std::move(uniform_block_name)){

    glGenBuffers(1, &ubo);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, bytes, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0); // unbind

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, ubo, 0, bytes);
}

void UniformBlock::SetBufferSubData(int offset, int size, void *value) {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, value);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void
UniformBlock::BindShader(Shader const &shader) {
    unsigned ub_index = glGetUniformBlockIndex(shader.id, uniformBlockName.c_str());
    glUniformBlockBinding(shader.id, ub_index, 0);
}

UniformBlock::~UniformBlock() { glDeleteBuffers(1, &ubo); }

UniformBlock::UniformBlock(UniformBlock &&rhs) noexcept:
    ubo(rhs.ubo), uniformBlockName(rhs.uniformBlockName), enabled(rhs.enabled)  {
    rhs.ubo = 0;
}
