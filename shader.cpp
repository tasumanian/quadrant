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

	glLinkProgram(m_program); //programをリンクs
    SDL_Log("Link requested");

    glDeleteShader(vs);
    glDeleteShader(fs);

    return CheckProgram(m_program);
}

GLuint Shader::GetProgram() const
{
    return m_program;
}
void Shader::Use() const
{
	glUseProgram(m_program); //programを使用
}
void Shader::SetVec3(const char* name, float x, float y, float z) const
{
	int location = glGetUniformLocation(m_program, name); // シェーダ内のuniform変数の場所を取得
    glUniform3f(location, x, y, z);
}


void Shader::SetMat4(const char* name, const glm::mat4& mat) const
{
	GLint location = // シェーダ内のuniform変数の場所を取得s
        glGetUniformLocation(
            m_program,
            name
        );

    glUniformMatrix4fv(
        location,
        1,
        GL_FALSE, //行列を転置しないs
		glm::value_ptr(mat) //glm::mat4をfloat配列に変換s
    );
}

bool Shader::CheckShader(unsigned int shader, unsigned int type) const
{
    int success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success); //コンパイルの成功を取得s

    if (!success)
    {
        int length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length); //エラーログの長さを取得s

        char* infoLog = new char[length];
		glGetShaderInfoLog(shader, length, nullptr, infoLog); //エラーログを取得s

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