#include "Mesh.h"

#include <glad/glad.h>
#include "MemoryTracker.h"
#include <cstddef>
#include <utility>

Mesh::~Mesh()
{
    Destroy();
}

Mesh::Mesh(Mesh&& other) noexcept
{
    *this = std::move(other);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    if (this != &other)
    {
        Destroy();
        name = std::move(other.name);
        m_vao = other.m_vao;
        m_vbo = other.m_vbo;
        m_ebo = other.m_ebo;
        m_indexCount = other.m_indexCount;
        m_gpuBytes = other.m_gpuBytes;
        m_hasBounds = other.m_hasBounds;
        m_boundsMin = other.m_boundsMin;
        m_boundsMax = other.m_boundsMax;

        other.m_vao = 0;
        other.m_vbo = 0;
        other.m_ebo = 0;
        other.m_indexCount = 0;
        other.m_gpuBytes = 0;
    }
    return *this;
}

void Mesh::Destroy()
{
    if (m_ebo) glDeleteBuffers(1, &m_ebo);
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
    m_vao = m_vbo = m_ebo = 0;
    m_indexCount = 0;
    m_hasBounds = false;

    if (m_gpuBytes)
    {
        MemTrack::Sub(MemTrack::Category::MeshGpu, m_gpuBytes);
        m_gpuBytes = 0;
    }
}

bool Mesh::Create(const Vertex* vertices, unsigned int vertexCount,
                  const unsigned int* indices, unsigned int indexCount)
{
    if (!vertices || !indices || vertexCount == 0 || indexCount == 0)
        return false;

    Destroy(); // safe to call Create again without leaking

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(vertexCount) * sizeof(Vertex),
                 vertices, GL_STATIC_DRAW);

    // The EBO binding is stored in the VAO, so keep it bound until the VAO is unbound.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(indexCount) * sizeof(unsigned int),
                 indices, GL_STATIC_DRAW);

    // location 0: position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, x));
    glEnableVertexAttribArray(0);

    // location 1: UV
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, u));
    glEnableVertexAttribArray(1);

    // location 2: normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, nx));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_indexCount = static_cast<int>(indexCount);

    // Local-space bounding box
    m_boundsMin = glm::vec3(vertices[0].x, vertices[0].y, vertices[0].z);
    m_boundsMax = m_boundsMin;
    for (unsigned int k = 1; k < vertexCount; ++k)
    {
        const glm::vec3 p(vertices[k].x, vertices[k].y, vertices[k].z);
        m_boundsMin = glm::min(m_boundsMin, p);
        m_boundsMax = glm::max(m_boundsMax, p);
    }
    m_hasBounds = true;

    m_gpuBytes = static_cast<unsigned long long>(vertexCount) * sizeof(Vertex)
               + static_cast<unsigned long long>(indexCount) * sizeof(unsigned int);
    MemTrack::Add(MemTrack::Category::MeshGpu, m_gpuBytes);

    return true;
}

bool Mesh::CreateCube()
{
    static const Vertex vertices[] =
    {
        // Front
        {-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0, 0, 1},
        { 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0, 0, 1},
        { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0, 0, 1},
        {-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0, 0, 1},
        // Back
        { 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0, 0,-1},
        {-0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0, 0,-1},
        {-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0, 0,-1},
        { 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0, 0,-1},
        // Right
        { 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1, 0, 0},
        { 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  1, 0, 0},
        { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1, 0, 0},
        { 0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  1, 0, 0},
        // Left
        {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1, 0, 0},
        {-0.5f, -0.5f,  0.5f,  1.0f, 0.0f, -1, 0, 0},
        {-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -1, 0, 0},
        {-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, -1, 0, 0},
        // Top
        {-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0, 1, 0},
        { 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0, 1, 0},
        { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0, 1, 0},
        {-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0, 1, 0},
        // Bottom
        {-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0,-1, 0},
        { 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0,-1, 0},
        { 0.5f, -0.5f,  0.5f,  1.0f, 1.0f,  0,-1, 0},
        {-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,  0,-1, 0},
    };

    static const unsigned int indices[] =
    {
         0,  1,  2,   2,  3,  0, // Front
         4,  5,  6,   6,  7,  4, // Back
         8,  9, 10,  10, 11,  8, // Right
        12, 13, 14,  14, 15, 12, // Left
        16, 17, 18,  18, 19, 16, // Top
        20, 21, 22,  22, 23, 20, // Bottom
    };

    return Create(vertices, static_cast<unsigned int>(sizeof(vertices) / sizeof(vertices[0])),
                  indices,  static_cast<unsigned int>(sizeof(indices)  / sizeof(indices[0])));
}

void Mesh::Draw() const
{
    if (m_vao == 0)
        return;

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
