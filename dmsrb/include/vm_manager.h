
#ifndef VM_MANAGER_H
#define VM_MANAGER_H

#include <stdio.h>
#include "memory.h"
#include "page_table.h"
#include "replacement.h"

int load_virtual_page(FILE *backing_store,
                      int page_number,
                      PhysicalMemory *memory,
                      PageTable *table,
                      FIFOReplacement *fifo,
                      LRUReplacement *lru,
                      int use_lru);

#endif
