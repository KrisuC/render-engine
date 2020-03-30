//
// Created by Krisu on 2020/2/29.
//

#include "Debug.hpp"
#include "StaticMesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "DirectionalShadow.hpp"

#ifdef ENGINE_DEBUG

void DEBUG_TEXTURE2D(unsigned texture) {
    static Shader debugShader {"shader/debug-texture.vert",
                               "shader/debug-texture.frag"};
    debugShader.UseShaderProgram();
    debugShader.Set("map", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    BuiltinMesh::RenderQuad();
}


#endif
