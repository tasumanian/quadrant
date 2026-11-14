#pragma once

#include <vector>
#include "Mesh.h"

// CPU-side mesh data for one terrain chunk.
// Building it does not touch OpenGL, so it can run on a worker thread later.
struct ChunkData
{
    std::vector<Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;
};

namespace Terrain
{
    constexpr int   kCells     = 32;                 // cells per side
    constexpr float kCellSize  = 1.0f;               // world units per cell
    constexpr float kChunkSize = kCells * kCellSize; // chunk width in world units

    // Terrain height at a world position (deterministic, pure function).
    float Height(float worldX, float worldZ);

    // Build vertices/indices for chunk (chunkX, chunkZ).
    // Vertex positions are LOCAL to the chunk origin:
    //   origin = (chunkX * kChunkSize, 0, chunkZ * kChunkSize)
    // Place the chunk's GameObject at that origin to get seamless neighbors.
    void BuildChunkData(int chunkX, int chunkZ, ChunkData& out);
}
