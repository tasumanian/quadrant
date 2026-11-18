#pragma once

// Custom allocators for learning and for the chunk streaming code.
//
//  - LinearAllocator : bump-pointer arena. Allocation = move a pointer.
//                      No per-object free; rewind/reset releases everything.
//                      NOT thread-safe: use one per thread.
//  - LinearScope     : RAII helper that rewinds a LinearAllocator on exit.
//  - PoolAllocator   : fixed-size blocks with an intrusive free list.
//                      Thread-safe (mutex).

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <new>
#include <type_traits>

#include "MemoryTracker.h"

namespace Alloc
{
    inline bool IsPowerOfTwo(std::size_t v)
    {
        return v != 0 && (v & (v - 1)) == 0;
    }

    // Round v up to the next multiple of align (align must be a power of two).
    inline std::size_t AlignUp(std::size_t v, std::size_t align)
    {
        return (v + align - 1) & ~(align - 1);
    }

    // ------------------------------------------------------------------
    // LinearAllocator
    // ------------------------------------------------------------------
    class LinearAllocator
    {
    public:
        explicit LinearAllocator(std::size_t capacity, std::size_t baseAlignment = 64)
            : m_capacity(capacity), m_baseAlignment(baseAlignment)
        {
            assert(IsPowerOfTwo(baseAlignment));
            m_base = static_cast<unsigned char*>(
                ::operator new(capacity, std::align_val_t(baseAlignment)));
        }

        ~LinearAllocator()
        {
            ::operator delete(m_base, std::align_val_t(m_baseAlignment));
        }

        LinearAllocator(const LinearAllocator&) = delete;
        LinearAllocator& operator=(const LinearAllocator&) = delete;

        // Returns nullptr when the arena is full.
        void* Allocate(std::size_t size, std::size_t align = alignof(std::max_align_t))
        {
            assert(IsPowerOfTwo(align));
            if (size > m_capacity)
                return nullptr;

            // Align the real address, not just the offset, so any alignment works.
            const std::uintptr_t base   = reinterpret_cast<std::uintptr_t>(m_base);
            const std::uintptr_t cursor = base + m_offset;
            const std::uintptr_t aligned =
                (cursor + (align - 1)) & ~static_cast<std::uintptr_t>(align - 1);

            const std::size_t newOffset = static_cast<std::size_t>(aligned - base) + size;
            if (newOffset > m_capacity)
                return nullptr;

            m_offset = newOffset;
            if (m_offset > m_peak)
                m_peak = m_offset;

            return reinterpret_cast<void*>(aligned);
        }

        // Uninitialized array of T. Destructors are never run, so T must be
        // trivially destructible.
        template <typename T>
        T* AllocateArray(std::size_t count)
        {
            static_assert(std::is_trivially_destructible<T>::value,
                          "LinearAllocator never runs destructors");
            return static_cast<T*>(Allocate(sizeof(T) * count, alignof(T)));
        }

        // Stack-like usage: remember a position, rewind to it later.
        std::size_t GetMarker() const { return m_offset; }
        void Rewind(std::size_t marker)
        {
            assert(marker <= m_offset);
            m_offset = marker;
        }

        // Frame-like usage: release everything at once (O(1)).
        void Reset() { m_offset = 0; }

        std::size_t Used() const { return m_offset; }
        std::size_t Peak() const { return m_peak; }
        std::size_t Capacity() const { return m_capacity; }

    private:
        unsigned char* m_base = nullptr;
        std::size_t m_capacity = 0;
        std::size_t m_baseAlignment = 64;
        std::size_t m_offset = 0;
        std::size_t m_peak = 0;
    };

    // Rewinds the allocator to where it was when the scope began.
    class LinearScope
    {
    public:
        explicit LinearScope(LinearAllocator& allocator)
            : m_allocator(allocator), m_marker(allocator.GetMarker())
        {
        }

        ~LinearScope() { m_allocator.Rewind(m_marker); }

        LinearScope(const LinearScope&) = delete;
        LinearScope& operator=(const LinearScope&) = delete;

    private:
        LinearAllocator& m_allocator;
        std::size_t m_marker;
    };

    // ------------------------------------------------------------------
    // PoolAllocator
    // ------------------------------------------------------------------
    class PoolAllocator
    {
    public:
        PoolAllocator(std::size_t blockSize,
                      std::size_t blockCount,
                      std::size_t alignment = 16,
                      MemTrack::Category category = MemTrack::Category::Other)
            : m_blockCount(blockCount), m_alignment(alignment), m_category(category)
        {
            assert(IsPowerOfTwo(alignment) && alignment >= alignof(FreeNode));

            // A free block stores the "next" pointer inside itself, so a block
            // must be at least pointer-sized. Round up so every block is aligned.
            m_blockSize = AlignUp(std::max(blockSize, sizeof(FreeNode)), alignment);

            m_memory = static_cast<unsigned char*>(
                ::operator new(m_blockSize * m_blockCount, std::align_val_t(alignment)));

            // Chain all blocks into the free list: block0 -> block1 -> ... -> null
            for (std::size_t i = 0; i < m_blockCount; ++i)
            {
                FreeNode* node = reinterpret_cast<FreeNode*>(m_memory + i * m_blockSize);
                node->next = (i + 1 < m_blockCount)
                    ? reinterpret_cast<FreeNode*>(m_memory + (i + 1) * m_blockSize)
                    : nullptr;
            }
            m_freeList = reinterpret_cast<FreeNode*>(m_memory);
        }

        ~PoolAllocator()
        {
            assert(m_used == 0 && "pool destroyed while blocks are still allocated");
            ::operator delete(m_memory, std::align_val_t(m_alignment));
        }

        PoolAllocator(const PoolAllocator&) = delete;
        PoolAllocator& operator=(const PoolAllocator&) = delete;

        // Returns nullptr when every block is in use.
        void* Allocate()
        {
            void* block = nullptr;
            {
                std::lock_guard<std::mutex> lock(m_mutex);
                if (!m_freeList)
                    return nullptr;

                FreeNode* node = m_freeList;
                m_freeList = node->next; // pop the head
                block = node;

                ++m_used;
                if (m_used > m_peak)
                    m_peak = m_used;
            }
            MemTrack::Add(m_category, m_blockSize);
            return block;
        }

        void Free(void* p)
        {
            if (!p)
                return;

            assert(Owns(p) && "pointer does not belong to this pool");
            assert((reinterpret_cast<std::uintptr_t>(p) -
                    reinterpret_cast<std::uintptr_t>(m_memory)) % m_blockSize == 0 &&
                   "pointer is not at a block boundary");

            {
                std::lock_guard<std::mutex> lock(m_mutex);
                FreeNode* node = static_cast<FreeNode*>(p);

#ifndef NDEBUG
                for (FreeNode* n = m_freeList; n; n = n->next)
                    assert(n != node && "double free");
#endif
                node->next = m_freeList; // push as the new head
                m_freeList = node;
                --m_used;
            }
            MemTrack::Sub(m_category, m_blockSize);
        }

        bool Owns(const void* p) const
        {
            const std::uintptr_t addr  = reinterpret_cast<std::uintptr_t>(p);
            const std::uintptr_t begin = reinterpret_cast<std::uintptr_t>(m_memory);
            return addr >= begin && addr < begin + m_blockSize * m_blockCount;
        }

        std::size_t BlockSize() const { return m_blockSize; }
        std::size_t BlockCount() const { return m_blockCount; }

        std::size_t Used() const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_used;
        }

        std::size_t Peak() const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_peak;
        }

    private:
        struct FreeNode
        {
            FreeNode* next;
        };

        unsigned char* m_memory = nullptr;
        std::size_t m_blockSize = 0;
        std::size_t m_blockCount = 0;
        std::size_t m_alignment = 16;
        MemTrack::Category m_category;

        mutable std::mutex m_mutex;
        FreeNode* m_freeList = nullptr;
        std::size_t m_used = 0;
        std::size_t m_peak = 0;
    };
}
