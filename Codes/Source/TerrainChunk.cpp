#include "TerrainChunk.h"

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>

namespace Terrain
{
    float Height(float worldX, float worldZ)
    {
        // Fractal noise: 4 octaves of Perlin noise.
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

    void BuildChunkData(int chunkX, int chunkZ, ChunkData& out)
    {
        const int vertsPerSide = kCells + 1;

        out.vertices.clear();
        out.indices.clear();
        out.vertices.reserve(static_cast<size_t>(vertsPerSide) * vertsPerSide);
        out.indices.reserve(static_cast<size_t>(kCells) * kCells * 6);

        const float originX = chunkX * kChunkSize;
        const float originZ = chunkZ * kChunkSize;
        const float e = kCellSize; // step for the finite difference

        for (int j = 0; j < vertsPerSide; ++j)
        {
            for (int i = 0; i < vertsPerSide; ++i)
            {
                const float lx = i * kCellSize;
                const float lz = j * kCellSize;
                const float wx = originX + lx;
                const float wz = originZ + lz;

                const float h = Height(wx, wz);

                // Normal from the height gradient (needs no neighbor chunk data).
                const float dhdx = (Height(wx + e, wz) - Height(wx - e, wz)) / (2.0f * e);
                const float dhdz = (Height(wx, wz + e) - Height(wx, wz - e)) / (2.0f * e);
                const glm::vec3 n = glm::normalize(glm::vec3(-dhdx, 1.0f, -dhdz));

                Mesh::Vertex v;
                v.x = lx;
                v.y = h;
                v.z = lz;
                v.u = wx * 0.25f; // texture repeats every 4 world units
                v.v = wz * 0.25f;
                v.nx = n.x;
                v.ny = n.y;
                v.nz = n.z;
                out.vertices.push_back(v);
            }
        }

        // Two triangles per cell, counter-clockwise when seen from +Y.
        for (int j = 0; j < kCells; ++j)
        {
            for (int i = 0; i < kCells; ++i)
            {
                const unsigned int a = j * vertsPerSide + i;           // (i,   j)
                const unsigned int b = (j + 1) * vertsPerSide + i;     // (i,   j+1)
                const unsigned int c = j * vertsPerSide + (i + 1);     // (i+1, j)
                const unsigned int d = (j + 1) * vertsPerSide + (i + 1); // (i+1, j+1)

                out.indices.push_back(a);
                out.indices.push_back(b);
                out.indices.push_back(c);

                out.indices.push_back(c);
                out.indices.push_back(b);
                out.indices.push_back(d);
            }
        }
    }
}
