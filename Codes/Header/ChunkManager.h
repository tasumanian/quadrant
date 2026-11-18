#pragma once

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <glm/glm.hpp>

#include "Allocators.h"
#include "Mesh.h"
#include "TerrainChunk.h"

class Scene;
class GameObject;
class Material;

// Keeps only the chunks around the focus position loaded, and picks a
// level of detail (LOD) for each chunk based on its distance.
//  - Worker threads build vertex/index data (CPU only, no OpenGL).
//    Each worker has its own scratch arena; results use pooled buffers.
//  - The main thread uploads finished data to the GPU and creates objects.
// Chunk meshes are owned here (not by ResourceManager) so they can be freed.
class ChunkManager
{
public:
    ChunkManager() = default;
    ~ChunkManager();

    ChunkManager(const ChunkManager&) = delete;
    ChunkManager& operator=(const ChunkManager&) = delete;

    void Init(Scene* scene, Material* material, float baseY = -10.0f);

    // Call once per frame on the main thread, outside any loop that
    // iterates scene objects.
    void Update(const glm::vec3& focusWorldPos);

    void UnloadAll();

    // Call between ImGui::NewFrame() and ImGui::Render()
    void DrawImGui(bool* open = nullptr);

    void SetLoadRadius(int radius);
    int  LoadedCount() const { return static_cast<int>(m_chunks.size()); }

private:
    struct Chunk
    {
        int x = 0;
        int z = 0;
        int lod = 0;
        std::unique_ptr<Mesh> mesh;
        GameObject* object = nullptr;
    };

    struct ChunkRequest
    {
        int x = 0;
        int z = 0;
        int lod = 0;
    };

    // Move-only (ChunkData owns a pool block).
    struct ChunkResult
    {
        int x = 0;
        int z = 0;
        int lod = 0;
        ChunkData data;
    };

    static long long MakeKey(int x, int z);

    // Distance from the focus point to the chunk center, in chunk units.
    float ChunkDistance(int x, int z) const;

    // LOD chosen for a distance. currentLod < 0 means "chunk not loaded yet".
    // For loaded chunks a margin is applied (hysteresis) to avoid flicker.
    int DesiredLod(float dist, int currentLod) const;

    // Create a new chunk object, or swap the mesh of an existing one.
    bool ApplyResult(ChunkResult& result); // main thread only (uses OpenGL)
    void UnloadChunk(long long key);

    void StartWorkers();
    void StopWorkers();
    void WorkerLoop();

    // Number of pooled buffers. In-flight requests are limited to 32 (see the
    // UI slider), so 64 blocks leave a comfortable margin.
    static constexpr std::size_t kPoolBlocks = 64;

    // Main-thread state
    Scene*    m_scene    = nullptr;
    Material* m_material = nullptr;
    float     m_baseY    = -10.0f;
    glm::vec3 m_focus    = glm::vec3(0.0f);

    int m_loadRadius       = 2; // chunks kept in each direction
    int m_maxLoadsPerFrame = 1; // GPU uploads per frame
    int m_maxOutstanding   = 8; // requests allowed in flight at once (max 32)

    // LOD settings (distances in chunk units)
    bool  m_lodEnabled = true;
    float m_lodDist[Terrain::kMaxLod] = { 1.5f, 3.5f, 6.5f }; // LOD0/1, LOD1/2, LOD2/3 borders
    float m_lodMargin  = 0.4f;

    int m_centerX = 0;
    int m_centerZ = 0;
    int m_missing = 0;

    // IMPORTANT: the pool must be declared BEFORE every container that holds
    // ChunkData. Members are destroyed in reverse order, so the pool is then
    // destroyed AFTER those containers have returned their blocks.
    std::unique_ptr<Alloc::PoolAllocator> m_chunkPool;

    std::unordered_map<long long, Chunk> m_chunks;
    std::unordered_set<long long> m_pendingKeys; // requested, not yet uploaded/discarded
    std::deque<ChunkResult> m_ready;             // built by workers, waiting for upload

    // Shared with workers (protected by m_mutex)
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::deque<ChunkRequest> m_requests;
    std::vector<ChunkResult> m_results;
    bool m_stop = false;
    std::vector<std::thread> m_threads;

    // Highest scratch-arena usage seen by any worker (for the UI).
    std::atomic<std::size_t> m_scratchPeak{ 0 };
};
