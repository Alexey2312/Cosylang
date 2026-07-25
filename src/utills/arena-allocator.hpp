#pragma once

#include <cstddef>
#include <algorithm>
#include <new>

class Arena
{
    struct Block
    {
        size_t size;
        size_t used;
        Block* next;

        Block(size_t block_size, Block* next_block) : size(block_size), used(0), next(next_block) {}

        char* data_ptr() noexcept
        {
            return reinterpret_cast<char*>(this + 1);
        }
    };

    Block* current_block = nullptr;
    size_t default_block_size = 1024 * 1024;

#if defined(__GNUC__) || defined(__clang__)
    __attribute__((noinline))
#elif defined(_MSC_VER)
    __declspec(noinline)
#endif
    void* allocate_new_block(size_t aligned_size)
    {
        size_t new_size = std::max(aligned_size, default_block_size);

        void* memory = ::operator new(sizeof(Block) + new_size);

        current_block = new (memory) Block(new_size, current_block);

        void* ptr = current_block->data_ptr() + current_block->used;
        current_block->used += aligned_size;
        return ptr;
    }

public:
    Arena() : default_block_size(1024 * 1024) {}

    explicit Arena(size_t block_size) : default_block_size(block_size) {}

    ~Arena() { release_all(); }

    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    Arena& operator=(Arena&& other) noexcept
    {
        if (this != &other)
        {
            release_all();
            current_block = other.current_block;
            default_block_size = other.default_block_size;
            other.current_block = nullptr;
            other.default_block_size = 0;
        }
        return *this;
    }

    void release_all() noexcept
    {
        Block* deleting_block = current_block;
        while (deleting_block)
        {
            Block* next = deleting_block->next;
            deleting_block->~Block();
            ::operator delete(deleting_block);
            deleting_block = next;
        }
        current_block = nullptr;
    }

    inline void* allocate(size_t object_size) noexcept
    {
        constexpr size_t alignment = alignof(std::max_align_t);
        size_t aligned_size = object_size;
        if (object_size % alignment != 0) {
            aligned_size = (object_size + alignment - 1) & ~(alignment - 1);
        }

        if (current_block && (current_block->used + aligned_size) <= current_block->size) [[likely]]
        {
            void* ptr = current_block->data_ptr() + current_block->used;
            current_block->used += aligned_size;
            return ptr;
        }

        return allocate_new_block(aligned_size);
    }

    void clear() noexcept
    {
        Block* resetting_block = current_block;
        while (resetting_block)
        {
            resetting_block->used = 0;
            resetting_block = resetting_block->next;
        }
    }
};

template<typename T>
class ArenaAllocator
{
public:
    Arena* arena;

    using value_type = T;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using void_pointer = void*;
    using const_void_pointer = const void*;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    using is_always_equal = std::false_type;

    ArenaAllocator(Arena* this_arena) : arena(this_arena) {}

    ArenaAllocator(const ArenaAllocator& other) noexcept : arena(other.arena) {}

    ArenaAllocator(ArenaAllocator&& other) noexcept : arena(other.arena) {}
    ~ArenaAllocator() = default;

    template <typename U>
    ArenaAllocator(const ArenaAllocator<U>& other) noexcept : arena(other.arena) {}

    ArenaAllocator& operator=(const ArenaAllocator& other) noexcept
    {
        if (this != &other)
        {
            arena = other.arena;
        }
        return *this;
    }

    T* allocate(size_t n)
    {
        if (n == 0)
        {
            return nullptr;
        }
        return static_cast<T*>(arena->allocate(n * sizeof(T)));
    }

    void deallocate(T* p, size_t n) noexcept
    {

    }

    template <typename U>
    bool operator==(const ArenaAllocator<U>& other) const noexcept
    {
        return arena == other.arena;
    }

    template <typename U>
    bool operator!=(const ArenaAllocator<U>& other) const noexcept
    {
        return arena != other.arena;
    }
};
