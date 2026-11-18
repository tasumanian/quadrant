#include "ChunkManager.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>

#include <imgui.h>

#include "Scene.h"
#include "GameObject.h"
#include "Components.h"
#include "Material.h"

ChunkManager::~ChunkManager()
{
    StopWorkers();

    // Return every pooled buffer while the pool still exists.
    m_ready.clear();
    m_results.clear();
}

void ChunkManager::Init(Scene* scene, Material* material, float baseY)
{
    m_scene = scene;
    m_material = material;
    m_baseY = baseY;

    // Reserve all chunk buffers up front. Must exist before the workers start.
    m_chunkPool = std::make_unique<Alloc::PoolAllocator>(
        Terrain::kChunkBlockBytes, kPoolBlocks, 16, MemTrack::Category::Chunk);

    StartWorkers();
}

long long ChunkManager::MakeKey(int x, int z)
{
    return (static_cast<long long>(x) << 32) ^ static_cast<long long>(static_cast<unsigned int>(z));
}

void ChunkManager::SetLoadRadius(int radius)
{
    m_loadRadius = std::max(1, std::min(radius, 16));
}

// ---------------------------------------------------------------
// LOD selection
// ---------------------------------------------------------------
float ChunkManager::ChunkDistance(int x, int z) const
{
    const float cx = (x + 0.5f) * Terrain::kChunkSize;
    const float cz = (z + 0.5f) * Terrain::kChunkSize;
    const float dx = std::fabs(m_focus.x - cx);
    const float dz = std::fabs(m_focus.z - cz);
    return std::max(dx, dz) / Terrain::kChunkSize; // Chebyshev distance in chunks
}

int ChunkManager::DesiredLod(float dist, int currentLod) const
{
    if (!m_lodEnabled)
        return 0;

    if (currentLod < 0)
    {
        // New chunk: plain threshold test.
        int lod = 0;
        while (lod < Terrain::kMaxLod && dist > m_lodDist[lod])
            ++lod;
        return lod;
    }

    // Loaded chunk: only change LOD when the distance is clearly past the
    // border (margin), so standing on a border does not cause rebuild flicker.
    int lod = currentLod;
    while (lod < Terrain::kMaxLod && dist > m_lodDist[lod] + m_lodMargin)
        ++lod;
    while (lod > 0 && dist < m_lodDist[lod - 1] - m_lodMargin)
        --lod;
    return lod;
}

// ---------------------------------------------------------------
// Worker threads
// ---------------------------------------------------------------
void ChunkManager::StartWorkers()
{
    if (!m_threads.empty())
        return;

    unsigned int hw = std::thread::hardware_concurrency();
    int count = hw > 1 ? static_cast<int>(hw) - 1 : 1;
    count = std::max(1, std::min(count, 4));

    m_stop = false;
    for (int i = 0; i < count; ++i)
        m_threads.emplace_back([this]() { WorkerLoop(); });
}

void ChunkManager::StopWorkers()
{
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stop = true;
    }
    m_cv.notify_all();

    for (std::thread& t : m_threads)
    {
        if (t.joinable())
            t.join();
    }
    m_threads.clear();
}

void ChunkManager::WorkerLoop()
{
    // Per-thread scratch arena: no locking needed, because only this thread
    // ever touches it.
    Alloc::LinearAllocator scratch(Terrain::kScratchBytes);

    for (;;)
    {
        ChunkRequest request;
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [this]() { return m_stop || !m_requests.empty(); });
            if (m_stop)
                return;

            request = m_requests.front();
            m_requests.pop_front();
        }

        // CPU-only work. Never call OpenGL from here.
        ChunkResult result;
        result.x = request.x;
        result.z = request.z;
        result.lod = request.lod;

        if (result.data.Acquire(*m_chunkPool))
        {
            if (!Terrain::BuildChunkData(request.x, request.z, request.lod,
                                         result.data, scratch))
            {
                result.data.Release(); // invalid result: the main thread discards it
            }

            // Track the highest scratch usage (atomic max).
            const std::size_t peak = scratch.Peak();
            std::size_t prev = m_scratchPeak.load();
            while (peak > prev && !m_scratchPeak.compare_exchange_weak(prev, peak))
            {
            }
        }
        // If the pool was empty, 'result' stays invalid and is discarded later.

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_results.push_back(std::move(result));
        }
    }
}

// ---------------------------------------------------------------
// Main thread
// ---------------------------------------------------------------
void ChunkManager::Update(const glm::vec3& focusWorldPos)
{
    if (!m_scene || !m_material)
        return;

    m_focus = focusWorldPos;
    m_centerX = static_cast<int>(std::floor(focusWorldPos.x / Terrain::kChunkSize));
    m_centerZ = static_cast<int>(std::floor(focusWorldPos.z / Terrain::kChunkSize));

    // radius + 1 gives hysteresis, so a chunk at the edge does not flip.
    const int unloadRadius = m_loadRadius + 1;

    // 1) Unload chunks that are too far.
    std::vector<long long> toUnload;
    for (auto& entry : m_chunks)
    {
        const Chunk& c = entry.second;
        if (std::abs(c.x - m_centerX) > unloadRadius ||
            std::abs(c.z - m_centerZ) > unloadRadius)
        {
            toUnload.push_back(entry.first);
        }
    }
    for (long long key : toUnload)
        UnloadChunk(key);

    // 2) Take finished results from the workers.
    {
        std::vector<ChunkResult> done;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            done.swap(m_results);
        }
        for (ChunkResult& r : done)
            m_ready.push_back(std::move(r));
    }

    // 3) Upload a limited number per frame (OpenGL, main thread only).
    //    'r' owns a pool block; it is returned automatically at the end of
    //    each iteration, whether the result was used or discarded.
    int uploaded = 0;
    while (!m_ready.empty() && uploaded < m_maxLoadsPerFrame)
    {
        ChunkResult r = std::move(m_ready.front());
        m_ready.pop_front();

        const long long key = MakeKey(r.x, r.z);
        m_pendingKeys.erase(key);

        // Build failed or the pool was empty: it will be requested again.
        if (!r.data.IsValid())
            continue;

        // The player may have moved away while this chunk was being built.
        const bool inRange = std::abs(r.x - m_centerX) <= unloadRadius &&
                             std::abs(r.z - m_centerZ) <= unloadRadius;
        if (!inRange)
            continue;

        auto it = m_chunks.find(key);
        const int currentLod = (it != m_chunks.end()) ? it->second.lod : -1;

        // Stale result: the wanted LOD changed while it was being built.
        if (r.lod != DesiredLod(ChunkDistance(r.x, r.z), currentLod))
            continue;

        // Already showing this LOD.
        if (currentLod == r.lod)
            continue;

        if (ApplyResult(r))
            ++uploaded;
    }

    // 4) Send requests (limited number in flight; nearest first).
    auto submit = [&](int x, int z, int lod) -> bool
    {
        if (static_cast<int>(m_pendingKeys.size()) >= m_maxOutstanding)
            return false;

        m_pendingKeys.insert(MakeKey(x, z));
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_requests.push_back({ x, z, lod });
        }
        m_cv.notify_one();
        return true;
    };

    // 4a) Chunks that are not loaded yet.
    struct Candidate { int x; int z; int lod; float dist; };
    std::vector<Candidate> missing;
    for (int dz = -m_loadRadius; dz <= m_loadRadius; ++dz)
    {
        for (int dx = -m_loadRadius; dx <= m_loadRadius; ++dx)
        {
            const int x = m_centerX + dx;
            const int z = m_centerZ + dz;
            if (m_chunks.find(MakeKey(x, z)) == m_chunks.end())
            {
                const float d = ChunkDistance(x, z);
                missing.push_back({ x, z, DesiredLod(d, -1), d });
            }
        }
    }
    std::sort(missing.begin(), missing.end(),
              [](const Candidate& a, const Candidate& b) { return a.dist < b.dist; });
    m_missing = static_cast<int>(missing.size());

    for (const Candidate& c : missing)
    {
        if (m_pendingKeys.find(MakeKey(c.x, c.z)) != m_pendingKeys.end())
            continue;
        if (!submit(c.x, c.z, c.lod))
            break; // keep the queue short so the nearest chunks stay first
    }

    // 4b) Loaded chunks whose LOD should change.
    std::vector<Candidate> changes;
    for (auto& entry : m_chunks)
    {
        const Chunk& c = entry.second;
        if (m_pendingKeys.find(entry.first) != m_pendingKeys.end())
            continue;

        const float d = ChunkDistance(c.x, c.z);
        const int want = DesiredLod(d, c.lod);
        if (want != c.lod)
            changes.push_back({ c.x, c.z, want, d });
    }
    std::sort(changes.begin(), changes.end(),
              [](const Candidate& a, const Candidate& b) { return a.dist < b.dist; });

    for (const Candidate& c : changes)
    {
        if (!submit(c.x, c.z, c.lod))
            break;
    }
}

bool ChunkManager::ApplyResult(ChunkResult& r)
{
    auto mesh = std::make_unique<Mesh>();

    // glBufferData copies the data, so the pooled buffer can be released
    // right after this call.
    if (!mesh->Create(r.data.vertices, r.data.vertexCount,
                      r.data.indices,  r.data.indexCount))
    {
        return false;
    }

    const long long key = MakeKey(r.x, r.z);
    auto it = m_chunks.find(key);

    if (it != m_chunks.end())
    {
        // Existing chunk: keep the GameObject, swap in the new mesh.
        Chunk& c = it->second;
        MeshRenderer* mr = c.object->GetComponent<MeshRenderer>();
        mr->mesh = mesh.get();
        c.mesh = std::move(mesh); // the old mesh is destroyed here (GPU memory freed)
        c.lod = r.lod;
        return true;
    }

    GameObject* obj = m_scene->CreateGameObject(
        "Chunk_" + std::to_string(r.x) + "_" + std::to_string(r.z));
    obj->GetTransform()->position = glm::vec3(r.x * Terrain::kChunkSize, m_baseY, r.z * Terrain::kChunkSize);
    obj->AddComponent<MeshRenderer>();

    MeshRenderer* mr = obj->GetComponent<MeshRenderer>();
    mr->mesh = mesh.get();
    mr->material = m_material;

    Chunk chunk;
    chunk.x = r.x;
    chunk.z = r.z;
    chunk.lod = r.lod;
    chunk.mesh = std::move(mesh);
    chunk.object = obj;
    m_chunks.emplace(key, std::move(chunk));
    return true;
}

void ChunkManager::UnloadChunk(long long key)
{
    auto it = m_chunks.find(key);
    if (it == m_chunks.end())
        return;

    // Destroy the object first so nothing refers to the mesh, then free the mesh.
    m_scene->DestroyGameObject(it->second.object);
    m_chunks.erase(it); // Mesh destructor releases the GPU buffers
}

void ChunkManager::UnloadAll()
{
    std::vector<long long> keys;
    for (auto& entry : m_chunks)
        keys.push_back(entry.first);
    for (long long key : keys)
        UnloadChunk(key);
}

void ChunkManager::DrawImGui(bool* open)
{
    if (!ImGui::Begin("Chunks", open))
    {
        ImGui::End();
        return;
    }

    // Statistics per LOD
    int lodCount[Terrain::kMaxLod + 1] = {};
    long long triangles = 0;
    for (auto& entry : m_chunks)
    {
        const Chunk& c = entry.second;
        ++lodCount[c.lod];
        triangles += c.mesh->GetIndexCount() / 3;
    }

    ImGui::Text("loaded    : %d", LoadedCount());
    ImGui::Text("missing   : %d", m_missing);
    ImGui::Text("in flight : %d", static_cast<int>(m_pendingKeys.size()));
    ImGui::Text("ready     : %d", static_cast<int>(m_ready.size()));
    ImGui::Text("center    : (%d, %d)", m_centerX, m_centerZ);
    ImGui::Text("workers   : %d", static_cast<int>(m_threads.size()));

    ImGui::Separator();
    ImGui::Text("LOD0:%d  LOD1:%d  LOD2:%d  LOD3:%d",
                lodCount[0], lodCount[1], lodCount[2], lodCount[3]);
    ImGui::Text("triangles : %lld", triangles);

    // Allocator statistics
    if (m_chunkPool)
    {
        ImGui::Separator();
        ImGui::Text("pool blocks : %d / %d (peak %d)",
                    static_cast<int>(m_chunkPool->Used()),
                    static_cast<int>(m_chunkPool->BlockCount()),
                    static_cast<int>(m_chunkPool->Peak()));
        ImGui::Text("block size  : %.1f KB", m_chunkPool->BlockSize() / 1024.0);
        ImGui::Text("scratch peak: %d / %d bytes (per worker)",
                    static_cast<int>(m_scratchPeak.load()),
                    static_cast<int>(Terrain::kScratchBytes));
    }

    ImGui::Separator();
    ImGui::Checkbox("enable LOD", &m_lodEnabled);
    ImGui::SliderFloat("LOD0 range", &m_lodDist[0], 0.5f, 16.0f);
    ImGui::SliderFloat("LOD1 range", &m_lodDist[1], 0.5f, 16.0f);
    ImGui::SliderFloat("LOD2 range", &m_lodDist[2], 0.5f, 16.0f);
    // Keep the borders in increasing order.
    m_lodDist[1] = std::max(m_lodDist[1], m_lodDist[0]);
    m_lodDist[2] = std::max(m_lodDist[2], m_lodDist[1]);

    ImGui::Separator();

    int radius = m_loadRadius;
    if (ImGui::SliderInt("load radius", &radius, 1, 16))
        SetLoadRadius(radius);

    ImGui::SliderInt("uploads per frame", &m_maxLoadsPerFrame, 1, 8);
    ImGui::SliderInt("max in flight", &m_maxOutstanding, 1, 32);

    ImGui::End();
}
