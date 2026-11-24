
#include "vm_manager.h"
#include "backing_store.h"

int load_virtual_page(FILE *backing_store,
                      int page_number,
                      PhysicalMemory *memory,
                      PageTable *table,
                      FIFOReplacement *fifo,
                      LRUReplacement *lru,
                      int use_lru)
{
    if (backing_store == NULL || memory == NULL || table == NULL ||
        fifo == NULL || lru == NULL ||
        page_number < 0 || page_number >= PAGE_COUNT) {
        return -1;
    }

    /* Verifica se a página já está na memória. */
    if (table->entries[page_number].valid) {
        int frame_number = table->entries[page_number].frame_number;

        if (use_lru) {
            lru_register_frame(lru, frame_number);
        }

        return frame_number;
    }

    /* Lê a página antes de modificar a memória. */
    signed char page_data[PAGE_SIZE];

    if (read_page(backing_store, page_number, page_data) != 0) {
        return -1;
    }

    /* Procura um frame livre. */
    int frame_number = find_free_frame(memory);

    if (frame_number == -1) {
        /* Escolhe uma vítima conforme o algoritmo. */
        if (use_lru) {
            frame_number = lru_select_victim(lru);
        } else {
            frame_number = fifo_select_victim(fifo);
        }

        if (frame_number == -1) {
            return -1;
        }

        /* Invalida a página que ocupava o frame escolhido. */
        for (int i = 0; i < PAGE_COUNT; i++) {
            if (table->entries[i].valid &&
                table->entries[i].frame_number == frame_number) {
                table->entries[i].valid = 0;
                table->entries[i].frame_number = -1;
                break;
            }
        }
    }

    /* Guarda se o frame estava livre antes de carregá-lo. */
    int frame_was_free = !memory->frame_used[frame_number];

    /* Carrega a nova página. */
    load_page(memory, frame_number, page_data);

    /* Atualiza a tabela de páginas. */
    table->entries[page_number].frame_number = frame_number;
    table->entries[page_number].valid = 1;

    /* Atualiza o algoritmo de substituição. */
    if (use_lru) {
        lru_register_frame(lru, frame_number);
    } else if (frame_was_free) {
        fifo_register_frame(fifo, frame_number);
    }

    return frame_number;
}
