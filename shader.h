#pragma once

#include "mat4.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    bool Create(const char* vertexSource, const char* fragmentSource);
    void Bind() const;
    void SetVec3(const char* name, float x, float y, float z) const;
    void SetMat4(const char* name, const glm::mat4& mat) const;
    GLuint GetProgram() const;

private:
    unsigned int Compile(unsigned int type, const char* source);
    bool CheckShader(unsigned int shader, unsigned int type) const;
    bool CheckProgram(unsigned int program) const;
    const char* GetShaderTypeName(unsigned int type) const;

private:
    unsigned int m_program = 0;
};