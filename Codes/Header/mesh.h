#pragma once
#include <string>
#include <glm/glm.hpp>

class Mesh
{
public:
    struct Vertex
    {
        float x, y, z;
        float u, v;
        float nx, ny, nz;
    };

    Mesh() = default;
    ~Mesh();

    // Owns GL resources: copy is disabled, move is allowed.
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    // Build a mesh from arbitrary vertex/index arrays (terrain chunks use this).
    bool Create(const Vertex* vertices, unsigned int vertexCount,
                const unsigned int* indices, unsigned int indexCount);

    // Compatibility: builds a cube.
    bool CreateCube();
    bool CreateObject() { return CreateCube(); }

    void Draw() const;
    void Destroy();

    bool IsValid() const { return m_vao != 0; }

    // Local-space bounding box (computed in Create)
    bool HasBounds() const { return m_hasBounds; }
    glm::vec3 GetBoundsMin() const { return m_boundsMin; }
    glm::vec3 GetBoundsMax() const { return m_boundsMax; }
    int  GetIndexCount() const { return m_indexCount; }

    std::string name;

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;
    int m_indexCount = 0;
    unsigned long long m_gpuBytes = 0; // reported to MemTrack

    bool m_hasBounds = false;
    glm::vec3 m_boundsMin = glm::vec3(0.0f);
    glm::vec3 m_boundsMax = glm::vec3(0.0f);
};
