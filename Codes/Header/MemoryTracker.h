#pragma once

// Simple memory tracker.
//  - CPU heap: every global operator new/delete is replaced and counted.
//  - GPU / other: call Add()/Sub() manually per category.

namespace MemTrack
{
    enum class Category
    {
        MeshGpu = 0,
        TextureGpu,
        Chunk,
        Other,
        Count
    };

    // Heap tracked through operator new/delete
    unsigned long long CurrentBytes();
    unsigned long long PeakBytes();
    unsigned long long LiveAllocations();
    unsigned long long TotalAllocations();

    // Manual categories (GPU buffers, chunk data, etc.)
    void Add(Category c, unsigned long long bytes);
    void Sub(Category c, unsigned long long bytes);
    unsigned long long CategoryBytes(Category c);
    const char* CategoryName(Category c);

    // Call between ImGui::NewFrame() and ImGui::Render()
    void DrawImGuiWindow(bool* open = nullptr);
}
