#pragma once

class Shader
{
public:
    bool Create(const char* vertexSource, const char* fragmentSource);
    void Bind() const;

private:
    unsigned int Compile(unsigned int type, const char* source);
    bool CheckShader(unsigned int shader) const;
    bool CheckProgram(unsigned int program) const;

private:
    unsigned int m_program = 0;
};