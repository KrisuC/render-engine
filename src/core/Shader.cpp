//
// Created by Krisu on 2019-11-18.
//

#include "Shader.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Debug.hpp"

Shader::Shader(const char *vertexPath,
               const char *fragmentPath,
               const char *geometryPath) {
    // 1. retrive the vertex/fragment src from filepath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;
    // ensure ifstream objects can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files.
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        if (geometryPath != nullptr) {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch(std::ifstream::failure &e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n";
        std::cerr << vertexPath << std::endl << geometryPath << std::endl;
        std::cerr << fragmentPath << std::endl;
        exit(-1);
    }
    const char * vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();

    // 2.compile shader
    unsigned int vertex, fragment;
    int success;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    checkCompileErrors(vertex, "VERTEX", vertexPath);

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    checkCompileErrors(fragment, "FRAGMENT", fragmentPath);

    // if geometry exists
    unsigned int geometry = 0;
    if (geometryPath != nullptr) {
        const char * gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, nullptr);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY", geometryPath);
    }

    // shader program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    if (geometryPath != nullptr)
        glAttachShader(id, geometry);
    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM", vertexPath);
    // delete shader
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometryPath != nullptr)
        glDeleteShader(geometry);
    DEBUG_LOG("Creaing Shader:", vertexPath, fragmentPath);
}

void Shader::UseShaderProgram() {
    glUseProgram(this->id);
}

void Shader::Set(const std::string &name, bool value) const {
    glUniform1i(getUniformLocation(name), (int)value);
}

void Shader::Set(const std::string &name, int value) const {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::Set(const std::string &name, unsigned value) const {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::Set(const std::string &name, float value) const {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::Set(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::Set(const std::string &name, float x, float y) const {
    glUniform2f(getUniformLocation(name), x, y);
}

void Shader::Set(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::Set(const std::string &name, float x, float y, float z) const {
    glUniform3f(getUniformLocation(name), x, y, z);
}

void Shader::Set(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::Set(const std::string &name, float x, float y, float z,
                 float w) const {
    glUniform4f(getUniformLocation(name), x, y, z, w);
}

void Shader::Set(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::Set(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::Set(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}



void
Shader::checkCompileErrors(GLuint shader, const std::string& type, std::string path) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << path << std::endl;
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of Type: " << type << "\n" << infoLog << "\n -- ------------------------------------ -- \n";
            exit(-1);
        }
        else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                path = path.substr(0, path.find('.'));
                glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
                std::cerr << path << std::endl;
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of Type: " << type << "\n" << infoLog << "\n --  ------------------------------------ -- " << std::endl;
                exit(-1);
            }
        }
    }
}

Shader::Shader() : Shader(
        "shader/default.vert",
        "shader/default.frag",
        nullptr) {

}

void Shader::SetLight(Light const &light, int light_index) {
    static const std::string lstr =
            "lights[" + std::to_string(light_index) + "]";

    this->Set(lstr + ".position", light.position);
    this->Set(lstr + ".direction", light.direction);
    this->Set(lstr + ".color", light.color);
    this->Set(lstr + ".cone_angle_in_radian", light.cone_angle_in_radian);

    this->Set("lights_cnt", light_index + 1);
    DEBUG_LOG("Adding light...", light_index);
}

Shader &Shader::DefaultShader() {
    static Shader default_shader;
    return default_shader;
}

Shader &Shader::TestShader() {
    static Shader test { "shader/test.vert",
                         "shader/test.frag" };
    return test;
}

void Shader::SetTransform(mat4 const& projection, mat4 const& view,
                          mat4 const& model) {
    this->Set("projection", projection);
    this->Set("view", view);
    this->Set("model", model);
}

void Shader::processShaderFile(char const *filePath, ShaderType shaderType) {
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(filePath);
        std::stringstream shaderss;
        shaderss << file.rdbuf();
        file.close();
        std::string code = shaderss.str();
    } catch(std::ifstream::failure& e) {
        // TODO
    }
}

void Shader::SetCameraPosition(Shader::vec3 cameraPosition) {
    Set("cameraPosition", cameraPosition);
}

int Shader::getUniformLocation(const Shader::string &name) const {
    int loc = glGetUniformLocation(this->id, name.c_str());
    if (loc == -1) {
        throw std::runtime_error("Shader uniform not exist: " + name);
    }
    return loc;
}
