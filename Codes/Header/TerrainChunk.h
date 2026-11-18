#pragma once

#include <cstddef>
#include "Mesh.h"

namespace Alloc
{
    class LinearAllocator;
    class PoolAllocator;
}

namespace Terrain
{
    constexpr int   kCells     = 32;                 // cells per side at LOD 0
    constexpr float kCellSize  = 1.0f;               // world units per cell at LOD 0
    constexpr float kChunkSize = kCells * kCellSize; // chunk width in world units

    // LOD 0 = finest. Each level doubles the sample spacing:
    //   LOD0: 32 cells, LOD1: 16 cells, LOD2: 8 cells, LOD3: 4 cells per side
    constexpr int   kMaxLod = 3;

    // Skirt: a wall hung below the chunk edges to hide cracks between
    // neighbors that use different LODs.
    constexpr float kSkirtDepth = 6.0f;

    inline int CellsForLod(int lod) { return kCells >> lod; }

    // ---- Buffer sizes (worst case = LOD 0) ----
    static_assert(sizeof(Mesh::Vertex) == 32, "Mesh::Vertex layout changed");

    constexpr unsigned int kMaxChunkVertices = (kCells + 1) * (kCells + 1) + 4 * kCells; // grid + skirt
    constexpr unsigned int kMaxChunkIndices  = kCells * kCells * 6 + 4 * kCells * 6;     // grid + skirt

    // One pool block holds [vertices][padding][indices].
    constexpr std::size_t kChunkIndexOffset =
        (kMaxChunkVertices * sizeof(Mesh::Vertex) + 15) & ~static_cast<std::size_t>(15);
    constexpr std::size_t kChunkBlockBytes =
        kChunkIndexOffset + kMaxChunkIndices * sizeof(unsigned int);

    // Scratch arena per worker thread (height grid + skirt table need ~10 KB).
    constexpr std::size_t kScratchBytes = 64 * 1024;
}

// CPU-side mesh data for one terrain chunk.
// The storage comes from a PoolAllocator block and is returned automatically
// when the object is destroyed (RAII). Move-only: exactly one owner at a time.
struct ChunkData
{
    ChunkData() = default;
    ~ChunkData() { Release(); }

    ChunkData(const ChunkData&) = delete;
    ChunkData& operator=(const ChunkData&) = delete;
    ChunkData(ChunkData&& other) noexcept;
    ChunkData& operator=(ChunkData&& other) noexcept;

    // Take one block from the pool. Returns false if the pool is empty.
    bool Acquire(Alloc::PoolAllocator& pool);

    // Give the block back (safe to call repeatedly).
    void Release();

    bool IsValid() const { return m_block != nullptr; }

    Mesh::Vertex* vertices = nullptr;
    unsigned int vertexCount = 0;
    unsigned int vertexCapacity = 0;

    unsigned int* indices = nullptr;
    unsigned int indexCount = 0;
    unsigned int indexCapacity = 0;

private:
    Alloc::PoolAllocator* m_pool = nullptr;
    void* m_block = nullptr;
};

namespace Terrain
{
    // Terrain height at a world position (deterministic, pure function).
    float Height(float worldX, float worldZ);

    // Build vertices/indices for chunk (chunkX, chunkZ) at the given LOD.
    //  - 'out' must already own a block (ChunkData::Acquire).
    //  - 'scratch' provides temporary memory; it is restored before returning.
    // Vertex positions are LOCAL to the chunk origin:
    //   origin = (chunkX * kChunkSize, 0, chunkZ * kChunkSize)
    // Returns false if memory ran out (the data must then be discarded).
    bool BuildChunkData(int chunkX, int chunkZ, int lod,
                        ChunkData& out, Alloc::LinearAllocator& scratch);
}
