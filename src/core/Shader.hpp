#ifndef shader_hpp
#define shader_hpp

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <string>

#include "UniformBlock.hpp"

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
    Shader(string const& vp, string const& fp, string const& gp = "");
    Shader(const GLchar *vertexPath,
           const GLchar *fragmentPath,
           const GLchar *geometryPath = nullptr);

    void UseShaderProgram();

    /* with check */
    template <typename... Args>
    void Set(const string& name, Args&& ...args) {
        this->UseShaderProgram();
        int loc = glGetUniformLocation(this->id, name.c_str());
        if (loc == -1) {
            throw std::runtime_error("Shader uniform not exist: " + name);
        }
        this->setUniform(loc, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void SetWithoutCheck(const string& name, Args&& ...args) {
        this->UseShaderProgram();
        int loc = glGetUniformLocation(this->id, name.c_str());
        this->setUniform(loc, std::forward<Args>(args)...);
    }

private:
    /* Uniform setting : const because setUniform will not change which shader program
     * Shader holds */
    void setUniform(int location, bool value) const;
    void setUniform(int location, int value) const;
    void setUniform(int location, unsigned value) const;
    void setUniform(int location, float value) const;
    void setUniform(int location, vec2 const& value) const;
    void setUniform(int location, float x, float y) const;
    void setUniform(int location, vec3 const& value) const;
    void setUniform(int location, float x, float y, float z) const;
    void setUniform(int location, vec4 const& value) const;
    void setUniform(int location, float x, float y, float z, float w) const;
    void setUniform(int location, mat2 const& mat) const;
    void setUniform(int location, mat3 const& mat) const;
    void setUniform(int location, mat4 const& mat) const;

private:
    // TODO:
    void processShaderFile(char const* filePath, ShaderType shaderType);

private:
    // The shader program id
    unsigned int id = 0;

    void checkCompileErrors(GLuint shader, const std::string &type,
                                           std::string path);

    friend class UniformBlock;
};



#endif /* shader_hpp */
