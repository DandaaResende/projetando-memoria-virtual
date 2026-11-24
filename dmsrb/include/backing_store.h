#ifndef BACKING_STORE_H
#define BACKING_STORE_H

#include <stdio.h>
#include "memory.h"

int read_page(FILE *backing_store, int page_number,
              signed char *page_data);

#endif