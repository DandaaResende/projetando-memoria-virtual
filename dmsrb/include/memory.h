#ifndef MEMORY_H
#define MEMORY_H

#define FRAME_COUNT 128
#define PAGE_SIZE 256
#define PHYSICAL_MEMORY_SIZE (FRAME_COUNT * PAGE_SIZE)

typedef struct {
    signed char bytes[PHYSICAL_MEMORY_SIZE];
    int frame_used[FRAME_COUNT];
} PhysicalMemory;

void initialize_memory(PhysicalMemory *memory);

#endif