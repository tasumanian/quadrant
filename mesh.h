#pragma once

class Mesh
{
public:
    bool CreateTriangle();
    void Draw() const;

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;
};