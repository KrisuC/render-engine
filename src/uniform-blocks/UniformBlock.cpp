//
// Created by Krisu on 2020/3/7.
//

#include "UniformBlock.hpp"

#include <utility>

#include "Shader.hpp"
#include "Debug.hpp"

// TODO: make current_binding update thread safe
int UniformBlock::current_total_binding = 0;

UniformBlock::UniformBlock(std::string uniform_block_name, int bytes)
        : uniform_block_name(std::move(uniform_block_name)),
          binding_point(current_total_binding++) {

    glGenBuffers(1, &ubo);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, bytes, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, binding_point, ubo, 0, bytes);

    DEBUG_LOG("Creating Uniform Buffer:", this->uniform_block_name, "with size:", bytes);
}

void UniformBlock::SetBufferSubData(int offset, int size, const void *value) {
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, value);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // DEBUG_LOG("Setting subdata of", uniform_block_name, "with offset:", offset, "size:", size);
}

void
UniformBlock::BindShader(Shader const &shader) {
    unsigned uniform_block_index = glGetUniformBlockIndex(shader.id, uniform_block_name.c_str());
    if (uniform_block_index == GL_INVALID_INDEX) {
        throw std::runtime_error("Failed to get uniform index");
    }
    glUniformBlockBinding(shader.id, uniform_block_index, binding_point);
}

UniformBlock::~UniformBlock() { glDeleteBuffers(1, &ubo); }

UniformBlock::UniformBlock(UniformBlock &&rhs) noexcept:
    ubo(rhs.ubo),
    uniform_block_name(std::move(rhs.uniform_block_name)),
    enabled(rhs.enabled),
    binding_point(rhs.binding_point)  {

    rhs.ubo = 0; // prevent from destructor free buffer storage
}
