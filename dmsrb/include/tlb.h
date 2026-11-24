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
    unsigned long last_used[TLB_SIZE];
    unsigned long clock;
} TLB;

void initialize_tlb(TLB *tlb);

int search_tlb(TLB *tlb, int page_number, int use_lru);

void insert_tlb(TLB *tlb, int page_number,
                int frame_number, int use_lru);

void invalidate_tlb_frame(TLB *tlb, int frame_number);

#endif
