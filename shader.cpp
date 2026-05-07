#include "shader.h"

#include <windows.h>
#include <glad/glad.h>
#include <SDL3/SDL.h>

unsigned int Shader::Compile(unsigned int type, const char* source)
{
    SDL_Log("Compiling %s Shader...", GetShaderTypeName(type));
    SDL_Log("%s", source);

    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    if (!CheckShader(shader, type))
    {
        glDeleteShader(shader);
        return 0;
    }

    SDL_Log("%s Shader Compile Success", GetShaderTypeName(type));
    return shader;
}

bool Shader::Create(const char* vertexSource, const char* fragmentSource)
{
    //頂点シェーダvs、フラグメントシェーダfsを生成s
    unsigned int vs = Compile(GL_VERTEX_SHADER, vertexSource);
    unsigned int fs = Compile(GL_FRAGMENT_SHADER, fragmentSource);

    if (!vs || !fs)
    {
        SDL_Log("Shader creation failed before link.");
        return false;
    }

    m_program = glCreateProgram(); //program生成s
    SDL_Log("Created Program = %u", m_program);

    glAttachShader(m_program, vs);//programにvsを統合
    SDL_Log("Attached Vertex Shader");

    glAttachShader(m_program, fs);//programにfsを統合
    SDL_Log("Attached Fragment Shader");

    glLinkProgram(m_program);
    SDL_Log("Link requested");

    glDeleteShader(vs);
    glDeleteShader(fs);

    return CheckProgram(m_program);
}

void Shader::Bind() const
{
    glUseProgram(m_program);

    GLint current = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current);

    //programが間違ってないか確認s
    SDL_Log("Current Program = %d", current);
}

void Shader::SetVec3(const char* name, float x, float y, float z) const
{
    int location = glGetUniformLocation(m_program, name);
    glUniform3f(location, x, y, z);
}


void Shader::SetMat4(const char* name, const Mat4& value) const
{
    SDL_Log("Program = %u", m_program);
    //uniform mat4 uModel(render.cpp,KVertexShader)を取得x
    GLint location = glGetUniformLocation(m_program, name);
    SDL_Log("Uniform %s location = %d", name, location);
    //行列をGPUに送信  GL_FALSEは"転置しないこと"
    glUniformMatrix4fv(location, 1, GL_FALSE, value.m);
}

bool Shader::CheckShader(unsigned int shader, unsigned int type) const
{
    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        int length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        char* infoLog = new char[length];
        glGetShaderInfoLog(shader, length, nullptr, infoLog);

        SDL_Log("%s Shader Compile Error:\n%s", GetShaderTypeName(type), infoLog);

        delete[] infoLog;
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
        int length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);

        char* infoLog = new char[length];
        glGetProgramInfoLog(program, length, nullptr, infoLog);

        SDL_Log("Program Link Error:\n%s", infoLog);

        delete[] infoLog;
        return false;
    }

    SDL_Log("Program Link Success");
    return true;
}
const char* Shader::GetShaderTypeName(unsigned int type) const
{
    switch (type)
    {
    case GL_VERTEX_SHADER:   return "Vertex";
    case GL_FRAGMENT_SHADER: return "Fragment";
    default:                 return "Unknown";
    }
}