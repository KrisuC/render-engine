//
// Created by Krisu on 2020/3/30.
//

#ifndef RENDER_ENGINE_MATERIAL_HPP
#define RENDER_ENGINE_MATERIAL_HPP

#include "Shader.hpp"
#include "Engine.hpp"

class Material {
public:
    virtual void UpdateShaderUniform() = 0;

    inline void SetShader(Shader &newShader) { pShader = &newShader; }
    inline Shader& GetShader() { return *pShader; }

private:
    Shader* pShader = nullptr;
};


#endif //RENDER_ENGINE_MATERIAL_HPP
