
#include <stddef.h>
#include "replacement.h"

/* FIFO */

void initialize_fifo(FIFOReplacement *fifo)
{
    fifo->head = 0;
    fifo->count = 0;
}

int fifo_register_frame(FIFOReplacement *fifo, int frame_number)
{
    if (fifo == NULL || frame_number < 0 ||
        frame_number >= FRAME_COUNT ||
        fifo->count >= FRAME_COUNT) {
        return -1;
    }

    int position = (fifo->head + fifo->count) % FRAME_COUNT;

    fifo->frame_order[position] = frame_number;
    fifo->count++;

    return 0;
}

int fifo_select_victim(FIFOReplacement *fifo)
{
    if (fifo == NULL || fifo->count < FRAME_COUNT) {
        return -1;
    }

    int victim = fifo->frame_order[fifo->head];

    fifo->head = (fifo->head + 1) % FRAME_COUNT;

    int position = (fifo->head + fifo->count - 1) % FRAME_COUNT;
    fifo->frame_order[position] = victim;

    return victim;
}

/* LRU */

void initialize_lru(LRUReplacement *lru)
{
    lru->clock = 0;

    for (int i = 0; i < FRAME_COUNT; i++) {
        lru->last_used[i] = 0;
    }
}

void lru_register_frame(LRUReplacement *lru, int frame_number)
{
    if (lru == NULL || frame_number < 0 ||
        frame_number >= FRAME_COUNT) {
        return;
    }

    lru->clock++;
    lru->last_used[frame_number] = lru->clock;
}

int lru_select_victim(LRUReplacement *lru)
{
    if (lru == NULL) {
        return -1;
    }

    int victim = -1;
    unsigned long oldest = (unsigned long)-1;

    for (int i = 0; i < FRAME_COUNT; i++) {
        if (lru->last_used[i] < oldest) {
            oldest = lru->last_used[i];
            victim = i;
        }
    }

    return victim;
}
