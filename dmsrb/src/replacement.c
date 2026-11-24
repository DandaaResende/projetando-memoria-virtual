#include <stddef.h>
#include "replacement.h"

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