#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
//これを定義することでstb_image.hの実装が有効になる

#include "stb_image.h"//画像読み込みライブラリ

Texture::Texture()
{
    m_texture = 0;
}
bool Texture::Load(const char* path)
{
    glGenTextures(1, &m_texture);

    glBindTexture(
        GL_TEXTURE_2D,
        m_texture
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_REPEAT
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );

    int width;
    int height;
    int channels;

    unsigned char* data =
        stbi_load(
            path,
            &width,
            &height,
            &channels,
            0
        );

    if (data)
    {
        GLenum format =
            channels == 4
            ? GL_RGBA
            : GL_RGB;

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            format,
            width,
            height,
            0,
            format,
            GL_UNSIGNED_BYTE,
            data
        );

        glGenerateMipmap(
            GL_TEXTURE_2D
        );

        stbi_image_free(data);

        return true;
    }

    return false;
}
void Texture::Bind() const
{
    glBindTexture(
        GL_TEXTURE_2D,
        m_texture
    );
}