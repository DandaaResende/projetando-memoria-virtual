#include "page_table.h"

void initialize_page_table(PageTable *table)
{
    for (int i = 0; i < PAGE_COUNT; i++) {
        table->entries[i].frame_number = -1;
        table->entries[i].valid = 0;
    }
}