#ifndef shader_hpp
#define shader_hpp

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <string>

#include "Light.hpp"
#include "Transform.hpp"

enum class ShaderType {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER,
    Geometry = GL_GEOMETRY_SHADER
};

class Shader {
    using vec3 = glm::vec3;
    using vec2 = glm::vec2;
    using vec4 = glm::vec4;
    using mat2 = glm::mat2;
    using mat3 = glm::mat3;
    using mat4 = glm::mat4;
    using string = std::string;

public:
    Shader(const GLchar *vertexPath,
           const GLchar *fragmentPath,
           const GLchar *geometryPath = nullptr);
    Shader();

    static Shader& DefaultShader();
    static Shader& TestShader();

    // "Activate" the shader
    void UseShaderProgram();

    // Uniform setting
    void Set(const string &name, bool value) const;
    void Set(const string &name, int value) const;
    void Set(const string &name, unsigned value) const;
    void Set(const string &name, float value) const;
    void Set(const string &name, vec2 const& value) const;
    void Set(const string &name, float x, float y) const;
    void Set(const string &name, vec3 const& value) const;
    void Set(const string &name, float x, float y, float z) const;
    void Set(const string &name, vec4 const& value) const;
    void Set(const string &name, float x, float y, float z, float w) const;
    void Set(const string &name, mat2 const& mat) const;
    void Set(const string &name, mat3 const& mat) const;
    void Set(const string &name, mat4 const& mat) const;

    // Specific Uniform Settings
    void SetLight(Light const& light);
    void SetTransform(mat4 const& projection, mat4 const& view,
                      mat4 const& model);
    void SetCameraPosition(vec3 cameraPosition);

private:
    // TODO:
    void processShaderFile(char const* filePath, ShaderType shaderType);

private:
    // The shader program id
    unsigned int id = 0;

    void checkCompileErrors(GLuint shader, const std::string &type,
                                           std::string path);
};

#endif /* shader_hpp */
