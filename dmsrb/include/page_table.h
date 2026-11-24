#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#define PAGE_COUNT 256

typedef struct {
    int frame_number;
    int valid;
} PageTableEntry;

typedef struct {
    PageTableEntry entries[PAGE_COUNT];
} PageTable;

void initialize_page_table(PageTable *table);

#endif