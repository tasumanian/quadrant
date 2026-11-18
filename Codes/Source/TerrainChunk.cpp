#include "TerrainChunk.h"

#include <algorithm>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

#include "Allocators.h"

// ---------------------------------------------------------------
// ChunkData (pool-backed storage)
// ---------------------------------------------------------------
ChunkData::ChunkData(ChunkData&& other) noexcept
{
    *this = std::move(other);
}

ChunkData& ChunkData::operator=(ChunkData&& other) noexcept
{
    if (this != &other)
    {
        Release(); // give back what we own before taking over

        vertices = other.vertices;
        vertexCount = other.vertexCount;
        vertexCapacity = other.vertexCapacity;
        indices = other.indices;
        indexCount = other.indexCount;
        indexCapacity = other.indexCapacity;
        m_pool = other.m_pool;
        m_block = other.m_block;

        // The source no longer owns anything.
        other.vertices = nullptr;
        other.vertexCount = other.vertexCapacity = 0;
        other.indices = nullptr;
        other.indexCount = other.indexCapacity = 0;
        other.m_pool = nullptr;
        other.m_block = nullptr;
    }
    return *this;
}

bool ChunkData::Acquire(Alloc::PoolAllocator& pool)
{
    Release();

    void* block = pool.Allocate();
    if (!block)
        return false;

    unsigned char* base = static_cast<unsigned char*>(block);

    m_pool = &pool;
    m_block = block;

    vertices = reinterpret_cast<Mesh::Vertex*>(base);
    vertexCount = 0;
    vertexCapacity = Terrain::kMaxChunkVertices;

    indices = reinterpret_cast<unsigned int*>(base + Terrain::kChunkIndexOffset);
    indexCount = 0;
    indexCapacity = Terrain::kMaxChunkIndices;
    return true;
}

void ChunkData::Release()
{
    if (m_block)
        m_pool->Free(m_block);

    m_pool = nullptr;
    m_block = nullptr;
    vertices = nullptr;
    vertexCount = vertexCapacity = 0;
    indices = nullptr;
    indexCount = indexCapacity = 0;
}

// ---------------------------------------------------------------
// Terrain
// ---------------------------------------------------------------
namespace Terrain
{
    float Height(float worldX, float worldZ)
    {
        // Fractal noise: 4 octaves of Perlin noise. Pure function, thread-safe.
        float h = 0.0f;
        float amplitude = 6.0f;
        float frequency = 0.02f;

        for (int octave = 0; octave < 4; ++octave)
        {
            h += glm::perlin(glm::vec2(worldX * frequency, worldZ * frequency)) * amplitude;
            amplitude *= 0.5f;
            frequency *= 2.0f;
        }
        return h;
    }

    bool BuildChunkData(int chunkX, int chunkZ, int lod,
                        ChunkData& out, Alloc::LinearAllocator& scratch)
    {
        if (!out.IsValid())
            return false;

        lod = std::max(0, std::min(lod, kMaxLod));
        out.vertexCount = 0;
        out.indexCount = 0;

        const int cells = CellsForLod(lod);            // cells per side at this LOD
        const float step = kCellSize * (1 << lod);     // world size of one cell
        const int vertsPerSide = cells + 1;
        const int paddedSide = vertsPerSide + 2;       // 1 extra ring for normals

        // Everything allocated from 'scratch' below is released when this
        // scope ends (the arena goes back to the state it had on entry).
        Alloc::LinearScope scope(scratch);

        float* heights = scratch.AllocateArray<float>(
            static_cast<std::size_t>(paddedSide) * paddedSide);
        if (!heights)
            return false;

        const float originX = chunkX * kChunkSize;
        const float originZ = chunkZ * kChunkSize;

        // Sample the height field once, with spacing = step (padded by 1 ring).
        for (int j = 0; j < paddedSide; ++j)
        {
            for (int i = 0; i < paddedSide; ++i)
            {
                heights[static_cast<std::size_t>(j) * paddedSide + i] =
                    Height(originX + (i - 1) * step, originZ + (j - 1) * step);
            }
        }

        // H(i, j) for i, j in [-1, cells + 1]
        auto H = [&](int i, int j)
        {
            return heights[static_cast<std::size_t>(j + 1) * paddedSide + (i + 1)];
        };

        bool overflow = false;

        auto pushVertex = [&](const Mesh::Vertex& v) -> unsigned int
        {
            if (out.vertexCount >= out.vertexCapacity)
            {
                overflow = true;
                return 0;
            }
            out.vertices[out.vertexCount] = v;
            return out.vertexCount++;
        };

        auto pushIndex = [&](unsigned int index)
        {
            if (out.indexCount >= out.indexCapacity)
            {
                overflow = true;
                return;
            }
            out.indices[out.indexCount++] = index;
        };

        // ---- Grid vertices ----
        for (int j = 0; j < vertsPerSide; ++j)
        {
            for (int i = 0; i < vertsPerSide; ++i)
            {
                const float lx = i * step;
                const float lz = j * step;

                // Normal from the height gradient (needs no neighbor chunk data).
                const float dhdx = (H(i + 1, j) - H(i - 1, j)) / (2.0f * step);
                const float dhdz = (H(i, j + 1) - H(i, j - 1)) / (2.0f * step);
                const glm::vec3 n = glm::normalize(glm::vec3(-dhdx, 1.0f, -dhdz));

                Mesh::Vertex v;
                v.x = lx;
                v.y = H(i, j);
                v.z = lz;
                v.u = (originX + lx) * 0.25f; // texture repeats every 4 world units
                v.v = (originZ + lz) * 0.25f;
                v.nx = n.x;
                v.ny = n.y;
                v.nz = n.z;
                pushVertex(v);
            }
        }

        // ---- Grid indices: two triangles per cell, CCW seen from +Y ----
        for (int j = 0; j < cells; ++j)
        {
            for (int i = 0; i < cells; ++i)
            {
                const unsigned int a = j * vertsPerSide + i;             // (i,   j)
                const unsigned int b = (j + 1) * vertsPerSide + i;       // (i,   j+1)
                const unsigned int c = j * vertsPerSide + (i + 1);       // (i+1, j)
                const unsigned int d = (j + 1) * vertsPerSide + (i + 1); // (i+1, j+1)

                pushIndex(a);
                pushIndex(b);
                pushIndex(c);

                pushIndex(c);
                pushIndex(b);
                pushIndex(d);
            }
        }

        // ---- Skirts: vertical walls hanging below the four edges ----
        const unsigned int kNone = 0xFFFFFFFFu;
        const unsigned int gridVertexCount = out.vertexCount;

        unsigned int* skirtOf = scratch.AllocateArray<unsigned int>(gridVertexCount);
        if (!skirtOf)
            return false;
        std::fill_n(skirtOf, gridVertexCount, kNone);

        // Lowered copy of a top vertex (created once, then reused).
        auto skirtVertex = [&](unsigned int top) -> unsigned int
        {
            if (skirtOf[top] == kNone)
            {
                Mesh::Vertex v = out.vertices[top];
                v.y -= kSkirtDepth;
                skirtOf[top] = pushVertex(v);
            }
            return skirtOf[top];
        };

        // Quad below the edge segment A -> B. The walk direction around the
        // chunk is chosen so that the wall faces outward.
        auto addSkirtQuad = [&](unsigned int a, unsigned int b)
        {
            const unsigned int a2 = skirtVertex(a);
            const unsigned int b2 = skirtVertex(b);

            pushIndex(a);
            pushIndex(b);
            pushIndex(b2);

            pushIndex(a);
            pushIndex(b2);
            pushIndex(a2);
        };

        auto V = [&](int i, int j) { return static_cast<unsigned int>(j * vertsPerSide + i); };

        for (int i = 0; i < cells; ++i)    addSkirtQuad(V(i, 0),     V(i + 1, 0));     // z = 0   edge, walking +x
        for (int j = 0; j < cells; ++j)    addSkirtQuad(V(cells, j), V(cells, j + 1)); // x = max edge, walking +z
        for (int i = cells; i >= 1; --i)   addSkirtQuad(V(i, cells), V(i - 1, cells)); // z = max edge, walking -x
        for (int j = cells; j >= 1; --j)   addSkirtQuad(V(0, j),     V(0, j - 1));     // x = 0   edge, walking -z

        return !overflow;
    }
}
