//
// Created by Krisu on 2020/3/7.
//

#ifndef RENDER_ENGINE_UNIFORMBLOCK_HPP
#define RENDER_ENGINE_UNIFORMBLOCK_HPP

#include "Renderer.hpp"

/*
 * shared uniform block across all shader instance
 */

class Shader;

class UniformBlock {
public:
    /* Generate uniform block buffer, bind buffer to the binding point */
    explicit UniformBlock(int bytes, int shader_binding_point,
                          std::string uniform_block_name);

    UniformBlock(UniformBlock const&) = delete;
    UniformBlock& operator=(UniformBlock const&) = delete;

    UniformBlock(UniformBlock && rhs) noexcept = default;

    /* Bind Shader's uniform block to the binding point */
    void BindShader(Shader const& shader);

    /* Setting buffer data */
    void SetBufferSubData(int offset, int size, void *value);

    ~UniformBlock();

private:
    unsigned ubo = 0;
    std::string uniform_block_name;
    /* for convenient...make it easier to enable/disable uniform block in Engine */
    bool enabled = false;
    int shader_binding_point;

    friend class Engine;
    friend class Shader;
};


#endif //RENDER_ENGINE_UNIFORMBLOCK_HPP
