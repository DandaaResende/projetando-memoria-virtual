#include "backing_store.h"

int read_page(FILE *backing_store, int page_number,
              signed char *page_data)
{
    if (backing_store == NULL || page_data == NULL ||
        page_number < 0 || page_number >= 256) {
        return -1;
    }

    long position = (long)page_number * PAGE_SIZE;

    if (fseek(backing_store, position, SEEK_SET) != 0) {
        return -1;
    }

    if (fread(page_data, sizeof(signed char), PAGE_SIZE,
              backing_store) != PAGE_SIZE) {
        return -1;
    }

    return 0;
}