
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

int find_free_frame(PhysicalMemory *memory)
{
    for (int i = 0; i < FRAME_COUNT; i++) {
        if (memory->frame_used[i] == 0) {
            return i;
        }
    }

    return -1;
}

void load_page(PhysicalMemory *memory, int frame_number,
               const signed char *page_data)
{
    int start = frame_number * PAGE_SIZE;

    for (int i = 0; i < PAGE_SIZE; i++) {
        memory->bytes[start + i] = page_data[i];
    }

    memory->frame_used[frame_number] = 1;
}
