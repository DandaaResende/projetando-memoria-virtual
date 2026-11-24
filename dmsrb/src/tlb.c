
#include <pthread.h>
#include "tlb.h"

typedef struct {
    TLBEntry *entry;
    int page_number;
    int frame_number;
} TLBSearchArgs;

/* Cada thread procura uma entrada da TLB. */
static void *search_tlb_entry(void *argument)
{
    TLBSearchArgs *args = argument;

    args->frame_number = -1;

    if (args->entry->valid &&
        args->entry->page_number == args->page_number) {
        args->frame_number = args->entry->frame_number;
    }

    return NULL;
}

void initialize_tlb(TLB *tlb)
{
    if (tlb == NULL) {
        return;
    }

    tlb->next = 0;
    tlb->clock = 0;

    for (int i = 0; i < TLB_SIZE; i++) {
        tlb->entries[i].page_number = -1;
        tlb->entries[i].frame_number = -1;
        tlb->entries[i].valid = 0;
        tlb->last_used[i] = 0;
    }
}

int search_tlb(TLB *tlb, int page_number, int use_lru)
{
    if (tlb == NULL || page_number < 0 || page_number >= 256) {
        return -1;
    }

    pthread_t threads[TLB_SIZE];
    TLBSearchArgs args[TLB_SIZE];

    int created = 0;

    /* Cria uma thread para cada entrada. */
    for (int i = 0; i < TLB_SIZE; i++) {
        args[i].entry = &tlb->entries[i];
        args[i].page_number = page_number;
        args[i].frame_number = -1;

        if (pthread_create(&threads[i], NULL,
                           search_tlb_entry, &args[i]) != 0) {
            break;
        }

        created++;
    }

    /* Aguarda todas as threads criadas. */
    for (int i = 0; i < created; i++) {
        pthread_join(threads[i], NULL);
    }

    if (created != TLB_SIZE) {
        return -1;
    }

    /* Reúne os resultados. */
    for (int i = 0; i < TLB_SIZE; i++) {
        if (args[i].frame_number != -1) {
            if (use_lru) {
                tlb->clock++;
                tlb->last_used[i] = tlb->clock;
            }

            return args[i].frame_number;
        }
    }

    return -1;
}

void insert_tlb(TLB *tlb, int page_number,
                int frame_number, int use_lru)
{
    if (tlb == NULL || page_number < 0 || page_number >= 256 ||
        frame_number < 0 || frame_number >= 128) {
        return;
    }

    int position = -1;

    /* Atualiza uma entrada existente, se houver. */
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb->entries[i].valid &&
            tlb->entries[i].page_number == page_number) {
            position = i;
            break;
        }
    }

    /* Procura uma posição vazia. */
    if (position == -1) {
        for (int i = 0; i < TLB_SIZE; i++) {
            if (!tlb->entries[i].valid) {
                position = i;
                break;
            }
        }
    }

    /* Se estiver cheia, escolhe uma vítima. */
    if (position == -1) {
        if (use_lru) {
            unsigned long oldest = (unsigned long)-1;

            for (int i = 0; i < TLB_SIZE; i++) {
                if (tlb->last_used[i] < oldest) {
                    oldest = tlb->last_used[i];
                    position = i;
                }
            }
        } else {
            position = tlb->next;
        }
    }

    tlb->entries[position].page_number = page_number;
    tlb->entries[position].frame_number = frame_number;
    tlb->entries[position].valid = 1;

    if (use_lru) {
        tlb->clock++;
        tlb->last_used[position] = tlb->clock;
    }

    /* Avança a fila FIFO. */
    if (!use_lru) {
        tlb->next = (position + 1) % TLB_SIZE;
    }
}

void invalidate_tlb_frame(TLB *tlb, int frame_number)
{
    if (tlb == NULL || frame_number < 0) {
        return;
    }

    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb->entries[i].valid &&
            tlb->entries[i].frame_number == frame_number) {

            tlb->entries[i].page_number = -1;
            tlb->entries[i].frame_number = -1;
            tlb->entries[i].valid = 0;
            tlb->last_used[i] = 0;
        }
    }
}

