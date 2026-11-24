
#include "tlb.h"

void initialize_tlb(TLB *tlb)
{
    tlb->next = 0;

    for (int i = 0; i < TLB_SIZE; i++) {
        tlb->entries[i].page_number = -1;
        tlb->entries[i].frame_number = -1;
        tlb->entries[i].valid = 0;
    }
}
