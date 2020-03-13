//
// Created by Krisu on 2020/3/7.
//

#ifndef RENDER_ENGINE_UNIFORMBLOCK_HPP
#define RENDER_ENGINE_UNIFORMBLOCK_HPP

#include <string>

#include "Renderer.hpp"

/*
 * shared uniform block across all shader instance
 */

class Shader;

class UniformBlock {
public:
    /* Generate uniform block buffer, bind buffer to the binding point */
    explicit UniformBlock(std::string uniform_block_name, int bytes);

    UniformBlock(UniformBlock const&) = delete;
    UniformBlock& operator=(UniformBlock const&) = delete;

    UniformBlock(UniformBlock && rhs) noexcept;

    /* Bind Shader's uniform block to the binding point */
    void BindShader(Shader const& shader);

    void SetEnabled(bool flag) { enabled = flag; }

    bool IsEnabled() const { return enabled; }

    ~UniformBlock();

protected:
    /* Setting buffer data */
    void SetBufferSubData(int offset, int size, const void *value);

private:
    unsigned ubo = 0;

    std::string uniform_block_name;
    /* for convenient...make it easier to enable/disable uniform block in Engine */
    bool enabled = false;

    int binding_point;

    static int current_total_binding;

    friend class Shader;
};


#endif //RENDER_ENGINE_UNIFORMBLOCK_HPP
