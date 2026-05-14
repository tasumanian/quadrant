#pragma once

class Mesh
{
public:
    bool CreateObject();
    void Draw() const;

    struct Vertex
    {
        float x;
        float y;
        float z;

        float u;
        float v;

        float nx;
        float ny;
        float nz;
    };

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;
};