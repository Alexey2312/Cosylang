#include "../../../external/silteli.hpp"
#include "../../../src/utills/arena-allocator.hpp"
#include <cassert>

UNIT_TEST(arena_allocation)
{
    {
        Arena arena(64);

        void* ptr1 = arena.allocate(1);
        void* ptr2 = arena.allocate(32);
        void* ptr3 = arena.allocate(17); // random size
    };

    Silteli::expect(true);
}
