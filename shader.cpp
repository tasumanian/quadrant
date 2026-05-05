#include "shader.h"

#include <windows.h>
#include <glad/glad.h>
#include <SDL3/SDL.h>

unsigned int Shader::Compile(unsigned int type, const char* source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    if (!CheckShader(shader))
    {
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool Shader::Create(const char* vertexSource, const char* fragmentSource)
{
    unsigned int vs = Compile(GL_VERTEX_SHADER, vertexSource); //頂点シェーダをコンパイル
    unsigned int fs = Compile(GL_FRAGMENT_SHADER, fragmentSource);//フラグメントシェーダをコンパイル

    if (!vs || !fs)
    {
        return false;
    }

    m_program = glCreateProgram(); //vsとfsをm_programに統合
    glAttachShader(m_program, vs);
    glAttachShader(m_program, fs);
    glLinkProgram(m_program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return CheckProgram(m_program);
}

void Shader::Bind() const
{
    glUseProgram(m_program);
}

bool Shader::CheckShader(unsigned int shader) const
{
    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        SDL_Log("Shader compile error: %s", infoLog); //エラー時用ログ
        return false;
    }

    return true;
}

bool Shader::CheckProgram(unsigned int program) const
{
    int success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        SDL_Log("Program link error: %s", infoLog);//エラー時用ログ
        return false;
    }

    return true;
}