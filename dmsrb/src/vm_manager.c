#include "vm_manager.h"
#include "backing_store.h"

int load_virtual_page(FILE *backing_store,
                      int page_number,
                      PhysicalMemory *memory,
                      PageTable *table)
{
    if (backing_store == NULL || memory == NULL || table == NULL ||
        page_number < 0 || page_number >= PAGE_COUNT) {
        return -1;
    }

    if (table->entries[page_number].valid) {
        return table->entries[page_number].frame_number;
    }

    int frame_number = find_free_frame(memory);

    if (frame_number == -1) {
        return -1;
    }

    signed char page_data[PAGE_SIZE];

    if (read_page(backing_store, page_number, page_data) != 0) {
        return -1;
    }

    load_page(memory, frame_number, page_data);

    table->entries[page_number].frame_number = frame_number;
    table->entries[page_number].valid = 1;

    return frame_number;
}