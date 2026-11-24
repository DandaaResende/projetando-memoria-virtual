
#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include "memory.h"

typedef struct {
    int frame_order[FRAME_COUNT];
    int head;
    int count;
} FIFOReplacement;

typedef struct {
    unsigned long last_used[FRAME_COUNT];
    unsigned long clock;
} LRUReplacement;

void initialize_fifo(FIFOReplacement *fifo);
int fifo_register_frame(FIFOReplacement *fifo, int frame_number);
int fifo_select_victim(FIFOReplacement *fifo);

void initialize_lru(LRUReplacement *lru);
void lru_register_frame(LRUReplacement *lru, int frame_number);
int lru_select_victim(LRUReplacement *lru);

#endif
