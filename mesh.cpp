#include "mesh.h"

#include <glad/glad.h>

bool Mesh::CreateObject()
{
    //次のs消すとエラー
    //座標を設定s 
    Vertex vertices[] =
    {
        // Front
        {-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0,0,1},
        { 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0,0,1},
        { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0,0,1},
        {-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0,0,1},

        // Back
        { 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 0,0,-1},
        {-0.5f, -0.5f, -0.5f,  1.0f,  0.0f, 0,0,-1},
        {-0.5f,  0.5f, -0.5f,  1.0f,  1.0f, 0,0,-1},
        { 0.5f,  0.5f, -0.5f,  0.0f,  1.0f, 0,0,-1},

        // Right
        { 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1,0,0},
        { 0.5f, -0.5f, -0.5f,  1.0f,  0.0f, 1,0,0},
        { 0.5f,  0.5f, -0.5f,  1.0f,  1.0f, 1,0,0},
        { 0.5f,  0.5f,  0.5f,  0.0f,  1.0f, 1,0,0},

        // Left
        {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,-1,0,0},
        {-0.5f, -0.5f,  0.5f,  1.0f, 0.0f,-1,0,0},
        {-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,-1,0,0},
        {-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,-1,0,0},

        // Top
        {-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,0,1,0},
        { 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,0,1,0},
        { 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,0,1,0},
        {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,0,1,0},

        // Bottom
        {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,0,-1,0},
        { 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,0,-1,0},
        { 0.5f, -0.5f,  0.5f,  1.0f,  1.0f,0,-1,0},
        {-0.5f, -0.5f,  0.5f,  0.0f,  1.0f,0,-1,0},
    };
    unsigned int indices[] = //各頂点の順番
    {
		//各面2つの三角形で構成されるため、6つの頂点が必要.
        // Front
       0, 1, 2,
       2, 3, 0,

       // Back
       4, 5, 6,
       6, 7, 4,

       // Right
       8, 9, 10,
       10, 11, 8,

       // Left
       12, 13, 14,
       14, 15, 12,

       // Top
       16, 17, 18,
       18, 19, 16,

       // Bottom
       20, 21, 22,
       22, 23, 20
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );
    glVertexAttribPointer( //xyzの読み込み
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    glVertexAttribPointer( //uvの読み込み
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);

    glVertexAttribPointer( //法線の読み込み
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)(5 * sizeof(float))
    );

    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    return true;
}

void Mesh::Draw() const
{
    glBindVertexArray(m_vao);
    glDrawElements(
        GL_TRIANGLES,
        36,
        GL_UNSIGNED_INT,
        nullptr
    );
}
