#include "Shader.h"
#include "Matrix.h"
#include "Diagnostics.h"

#include <glad/gl.h>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode) {

    unsigned int vertex, fragment, geometry;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    errorCheck(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    errorCheck(fragment, "FRAGMENT");

    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);

    // if geometry shader is given, compile geometry shader
    if (gShaderCode != nullptr) {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        errorCheck(geometry, "GEOMETRY");
        
        glAttachShader(id, geometry);
        glDeleteShader(geometry);
    }

    glLinkProgram(id);
    errorCheck(id, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::activate() {
    glUseProgram(id);
}

void Shader::setInt(const std::string& name, const int& value) {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const Mat4& mat) {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setFloat(const std::string& name, const float& value) {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::errorCheck(size_t shader, const char* type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            Diagnostics::Throw("Shader syntax error: {}", infoLog);
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            Diagnostics::Throw("Shader syntax error: {}", infoLog);
        }
    }
}
