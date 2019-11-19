#ifndef shader_hpp
#define shader_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <string>

class Shader {
public:
    unsigned int getID();
    // constructor reads and builds the shader
    Shader(const GLchar *vertexPath,
           const GLchar *fragmentPath,
           const GLchar *geometryPath = nullptr);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    // the program id
    unsigned int ID;
    // checking error
    void checkCompileErrors(GLuint shader, const std::string& type, std::string path);
};



#endif /* shader_hpp */
