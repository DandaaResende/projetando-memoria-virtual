
#ifndef TLB_H
#define TLB_H

#define TLB_SIZE 16

typedef struct {
    int page_number;
    int frame_number;
    int valid;
} TLBEntry;

typedef struct {
    TLBEntry entries[TLB_SIZE];
    int next;
} TLB;

void initialize_tlb(TLB *tlb);

#endif
