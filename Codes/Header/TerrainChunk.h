#pragma once

#include <vector>
#include "Mesh.h"

// CPU-side mesh data for one terrain chunk.
// Building it does not touch OpenGL, so it can run on a worker thread.
struct ChunkData
{
    std::vector<Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;
};

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

    // Terrain height at a world position (deterministic, pure function).
    float Height(float worldX, float worldZ);

    // Build vertices/indices for chunk (chunkX, chunkZ) at the given LOD.
    // Vertex positions are LOCAL to the chunk origin:
    //   origin = (chunkX * kChunkSize, 0, chunkZ * kChunkSize)
    void BuildChunkData(int chunkX, int chunkZ, ChunkData& out, int lod = 0);
}
