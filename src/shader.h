
#ifndef CHIP8EMULATOR_SHADER_H
#define CHIP8EMULATOR_SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    Shader() = default;

    void LoadShaderProgram(const char *vertexFile, const char *fragmentFile);

    void Use(){ glUseProgram(shaderID); }

    void SetBool (const char *name, int value){
        glUniform1i(glGetUniformLocation(shaderID, name), value);
    }

    void SetInt(const char *name, int value){
        glUniform1i(glGetUniformLocation(shaderID, name), value);
    }

    void SetFloat(const char *name, float value){
        glUniform1f(glGetUniformLocation(shaderID, name), value);
    }

    void SetVec2(const char *name, const glm::vec2 &value){
        glUniform2fv(glGetUniformLocation(shaderID, name), 1, &value[0]);
    }

    void SetVec2(const char *name, float x, float y){
        glUniform2f(glGetUniformLocation(shaderID, name), x, y);
    }

    void SetVec3(const char *name, glm::vec3 &value){
        glUniform3fv(glGetUniformLocation(shaderID, name), 1, &value[0]);
    }
    void SetVec3(const char *name, float x, float y, float z){
        glUniform3f(glGetUniformLocation(shaderID, name), x, y, z);
    }
    // ------------------------------------------------------------------------
    void SetVec4(const char *name, glm::vec4 &value){
        glUniform4fv(glGetUniformLocation(shaderID, name), 1, &value[0]);
    }
    void SetVec4(const char *name, float x, float y, float z, float w){
        glUniform4f(glGetUniformLocation(shaderID, name), x, y, z, w);
    }

    void SetMat2(const char *name, glm::mat2 &mat){
        glUniformMatrix2fv(glGetUniformLocation(shaderID, name), 1, GL_FALSE, &mat[0][0]);
    }

    void SetMat3(const char *name, glm::mat3 &mat){
        glUniformMatrix3fv(glGetUniformLocation(shaderID, name), 1, GL_FALSE, &mat[0][0]);
    }

    void SetMat4(const char *name, glm::mat4 &mat){
        glUniformMatrix4fv(glGetUniformLocation(shaderID, name), 1, GL_FALSE, &mat[0][0]);
    }

private:
    void checkCompileErrors(GLuint shader, const char* type);

public:
    unsigned int shaderID;
};


#endif //CHIP8EMULATOR_SHADER_H
