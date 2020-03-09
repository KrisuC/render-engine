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
    /* Generate uniform block buffer */
    explicit UniformBlock(int bytes, int binding_point=0);

    /* Bind Shader's uniform block to this buffer */
    void BindShader(Shader const& shader, const char* uniform_block_name);

    /* Setting buffer data */
    void SetBufferSubData(int offset, int size, float *value);

private:
    unsigned ubo;

    friend class Shader;
};


#endif //RENDER_ENGINE_UNIFORMBLOCK_HPP
