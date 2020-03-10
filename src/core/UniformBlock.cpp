//
// Created by Krisu on 2020/3/7.
//

#include "UniformBlock.hpp"

#include <utility>

#include "Shader.hpp"
#include "Debug.hpp"

UniformBlock::UniformBlock(int bytes, int shader_binding_point,
                           std::string uniform_block_name)
        : uniform_block_name(std::move(uniform_block_name)),
          shader_binding_point(shader_binding_point) {

    glGenBuffers(1, &ubo);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, bytes, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, shader_binding_point, ubo, 0, bytes);

    DEBUG_LOG("Creating Uniform Buffer:", this->uniform_block_name, "with size:", bytes);
}

void UniformBlock::SetBufferSubData(int offset, int size, void *value) {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, value);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // DEBUG_LOG("Setting subdata of", uniformBlockName, "with offset:", offset, "size:", size);
}

void
UniformBlock::BindShader(Shader const &shader) {
    unsigned uniform_block_index = glGetUniformBlockIndex(shader.id, uniform_block_name.c_str());
    if (uniform_block_index == GL_INVALID_INDEX) {
        throw std::runtime_error("Failed to get uniform index");
    }
    glUniformBlockBinding(shader.id, uniform_block_index, shader_binding_point);
}

UniformBlock::~UniformBlock() { glDeleteBuffers(1, &ubo); }

//UniformBlock::UniformBlock(UniformBlock &&rhs) noexcept:
//    ubo(rhs.ubo), uniformBlockName(rhs.uniformBlockName), enabled(rhs.enabled), shader_binding_point(rhs.shader_binding_point)  {
//    rhs.ubo = 0;
//}
