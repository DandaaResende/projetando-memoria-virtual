#ifndef VM_MANAGER_H
#define VM_MANAGER_H

#include <stdio.h>
#include "memory.h"
#include "page_table.h"

int load_virtual_page(FILE *backing_store,
                      int page_number,
                      PhysicalMemory *memory,
                      PageTable *table);

#endif