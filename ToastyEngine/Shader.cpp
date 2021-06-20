#include "Shader.h"
#include "Matrix.h"
#include "Diagnostics.h"

#include <glad/gl.h>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const char* vertPath, const char* fragPath, const char* geoPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertPath);
        fShaderFile.open(fragPath);
        std::stringstream vShaderStream, fShaderStream;

        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // close file handlers
        vShaderFile.close();
        fShaderFile.close();

        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        // if geometry shader path is present, also load a geometry shader
        if (geoPath != nullptr)
        {
            gShaderFile.open(geoPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::ifstream::failure& err) {
        Diagnostics::Throw("Shader file could not be read: {}.", err.what());
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;

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

    // if geometry shader is given, compile geometry shader
    unsigned int geometry;
    if (geoPath != nullptr)
    {
        const char* gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        errorCheck(geometry, "GEOMETRY");
    }

    // shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    if (geoPath != nullptr)
        glAttachShader(id, geometry);
    glLinkProgram(id);
    errorCheck(id, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geoPath != nullptr)
        glDeleteShader(geometry);
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
