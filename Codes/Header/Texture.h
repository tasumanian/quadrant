#pragma once

#include <glad/glad.h>

class Texture
{
private:

    GLuint m_texture;

public:

    Texture();

    bool Load(const char* path);

    void Bind() const;
};