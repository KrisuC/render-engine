//
// Created by Krisu on 2020/2/29.
//

#include "Debug.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Shadow.hpp"

#ifdef ENGINE_DEBUG

void DEBUG_TEXTURE2D(const Texture &t) {
    static Shader debugShader {"shader/debug-texture.vert",
                               "shader/debug-texture.frag"};
    debugShader.UseShaderProgram();
    debugShader.Set("map", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(static_cast<int>(t.Type()), t.ID());
    SimpleMesh::renderQuad();
}

void DEBUG_SHADOW_MAP(const Shadow &s) {
    static Shader debugShader {"shader/debug-texture.vert",
                               "shader/debug-shadow-map.frag"};
    debugShader.UseShaderProgram();
    debugShader.Set("map", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s.depthMap);
    SimpleMesh::renderQuad();
}


#endif
