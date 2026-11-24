#include "memory.h"

void initialize_memory(PhysicalMemory *memory)
{
    for (int i = 0; i < PHYSICAL_MEMORY_SIZE; i++) {
        memory->bytes[i] = 0;
    }

    for (int i = 0; i < FRAME_COUNT; i++) {
        memory->frame_used[i] = 0;
    }
}