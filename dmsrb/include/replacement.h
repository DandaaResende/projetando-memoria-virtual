#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include "memory.h"

typedef struct {
    int frame_order[FRAME_COUNT];
    int head;
    int count;
} FIFOReplacement;

void initialize_fifo(FIFOReplacement *fifo);

int fifo_register_frame(FIFOReplacement *fifo, int frame_number);

int fifo_select_victim(FIFOReplacement *fifo);

#endif