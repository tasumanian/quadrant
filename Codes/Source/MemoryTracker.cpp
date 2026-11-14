#include "MemoryTracker.h"

#include <atomic>
#include <cfloat>
#include <cstdlib>
#include <malloc.h>
#include <new>

#include <imgui.h>

namespace
{
    // Constant-initialized, so safe to use before main() and from any thread.
    std::atomic<unsigned long long> g_current{0};
    std::atomic<unsigned long long> g_peak{0};
    std::atomic<unsigned long long> g_live{0};
    std::atomic<unsigned long long> g_total{0};
    std::atomic<unsigned long long> g_category[static_cast<int>(MemTrack::Category::Count)] = {};

    void OnAlloc(std::size_t n)
    {
        unsigned long long now = g_current.fetch_add(n) + n;
        g_live.fetch_add(1);
        g_total.fetch_add(1);

        unsigned long long peak = g_peak.load();
        while (now > peak && !g_peak.compare_exchange_weak(peak, now))
        {
        }
    }

    void OnFree(std::size_t n)
    {
        g_current.fetch_sub(n);
        g_live.fetch_sub(1);
    }

    void* AllocImpl(std::size_t size)
    {
        if (size == 0) size = 1;
        for (;;)
        {
            void* p = std::malloc(size);
            if (p)
            {
                OnAlloc(_msize(p));
                return p;
            }
            std::new_handler h = std::get_new_handler();
            if (!h) return nullptr;
            h();
        }
    }

    void FreeImpl(void* p)
    {
        if (!p) return;
        OnFree(_msize(p));
        std::free(p);
    }

    void* AlignedAllocImpl(std::size_t size, std::size_t align)
    {
        if (size == 0) size = 1;
        void* p = _aligned_malloc(size, align);
        if (p) OnAlloc(_aligned_msize(p, align, 0));
        return p;
    }

    void AlignedFreeImpl(void* p, std::size_t align)
    {
        if (!p) return;
        OnFree(_aligned_msize(p, align, 0));
        _aligned_free(p);
    }
}

// ---------------------------------------------------------------
// Public API
// ---------------------------------------------------------------
namespace MemTrack
{
    unsigned long long CurrentBytes()      { return g_current.load(); }
    unsigned long long PeakBytes()         { return g_peak.load(); }
    unsigned long long LiveAllocations()   { return g_live.load(); }
    unsigned long long TotalAllocations()  { return g_total.load(); }

    void Add(Category c, unsigned long long bytes)
    {
        g_category[static_cast<int>(c)].fetch_add(bytes);
    }

    void Sub(Category c, unsigned long long bytes)
    {
        g_category[static_cast<int>(c)].fetch_sub(bytes);
    }

    unsigned long long CategoryBytes(Category c)
    {
        return g_category[static_cast<int>(c)].load();
    }

    const char* CategoryName(Category c)
    {
        switch (c)
        {
        case Category::MeshGpu:    return "Mesh (GPU)";
        case Category::TextureGpu: return "Texture (GPU)";
        case Category::Chunk:      return "Chunk";
        case Category::Other:      return "Other";
        default:                   return "?";
        }
    }

    void DrawImGuiWindow(bool* open)
    {
        constexpr int kHistory = 240;
        static float history[kHistory] = {};
        static int offset = 0;
        const double kMB = 1024.0 * 1024.0;

        // One sample per call (= per frame)
        history[offset] = static_cast<float>(CurrentBytes() / kMB);
        offset = (offset + 1) % kHistory;

        if (!ImGui::Begin("Memory", open))
        {
            ImGui::End();
            return;
        }

        ImGui::Text("CPU heap (operator new)");
        ImGui::Text("  current : %.3f MB", CurrentBytes() / kMB);
        ImGui::Text("  peak    : %.3f MB", PeakBytes() / kMB);
        ImGui::Text("  live allocations : %llu", LiveAllocations());
        ImGui::Text("  total allocations: %llu", TotalAllocations());

        ImGui::PlotLines("##heap_history", history, kHistory, offset,
                         "heap (MB)", 0.0f, FLT_MAX, ImVec2(0, 80));

        ImGui::Separator();
        ImGui::Text("Categories");
        for (int i = 0; i < static_cast<int>(Category::Count); ++i)
        {
            Category c = static_cast<Category>(i);
            ImGui::Text("  %-14s: %.3f MB", CategoryName(c), CategoryBytes(c) / kMB);
        }

        ImGui::End();
    }
}

// ---------------------------------------------------------------
// Global operator new / delete replacement
// ---------------------------------------------------------------
void* operator new(std::size_t size)
{
    void* p = AllocImpl(size);
    if (!p) throw std::bad_alloc();
    return p;
}

void* operator new[](std::size_t size)
{
    void* p = AllocImpl(size);
    if (!p) throw std::bad_alloc();
    return p;
}

void* operator new(std::size_t size, const std::nothrow_t&) noexcept
{
    return AllocImpl(size);
}

void* operator new[](std::size_t size, const std::nothrow_t&) noexcept
{
    return AllocImpl(size);
}

void operator delete(void* p) noexcept                              { FreeImpl(p); }
void operator delete[](void* p) noexcept                            { FreeImpl(p); }
void operator delete(void* p, std::size_t) noexcept                 { FreeImpl(p); }
void operator delete[](void* p, std::size_t) noexcept               { FreeImpl(p); }
void operator delete(void* p, const std::nothrow_t&) noexcept       { FreeImpl(p); }
void operator delete[](void* p, const std::nothrow_t&) noexcept     { FreeImpl(p); }

// Over-aligned versions (C++17)
void* operator new(std::size_t size, std::align_val_t align)
{
    void* p = AlignedAllocImpl(size, static_cast<std::size_t>(align));
    if (!p) throw std::bad_alloc();
    return p;
}

void* operator new[](std::size_t size, std::align_val_t align)
{
    void* p = AlignedAllocImpl(size, static_cast<std::size_t>(align));
    if (!p) throw std::bad_alloc();
    return p;
}

void* operator new(std::size_t size, std::align_val_t align, const std::nothrow_t&) noexcept
{
    return AlignedAllocImpl(size, static_cast<std::size_t>(align));
}

void* operator new[](std::size_t size, std::align_val_t align, const std::nothrow_t&) noexcept
{
    return AlignedAllocImpl(size, static_cast<std::size_t>(align));
}

void operator delete(void* p, std::align_val_t align) noexcept
{
    AlignedFreeImpl(p, static_cast<std::size_t>(align));
}

void operator delete[](void* p, std::align_val_t align) noexcept
{
    AlignedFreeImpl(p, static_cast<std::size_t>(align));
}

void operator delete(void* p, std::size_t, std::align_val_t align) noexcept
{
    AlignedFreeImpl(p, static_cast<std::size_t>(align));
}

void operator delete[](void* p, std::size_t, std::align_val_t align) noexcept
{
    AlignedFreeImpl(p, static_cast<std::size_t>(align));
}

void operator delete(void* p, std::align_val_t align, const std::nothrow_t&) noexcept
{
    AlignedFreeImpl(p, static_cast<std::size_t>(align));
}

void operator delete[](void* p, std::align_val_t align, const std::nothrow_t&) noexcept
{
    AlignedFreeImpl(p, static_cast<std::size_t>(align));
}
