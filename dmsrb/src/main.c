
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "address.h"
#include "memory.h"
#include "page_table.h"
#include "backing_store.h"
#include "vm_manager.h"
#include "replacement.h"
#include "tlb.h"

#define MAX_VIRTUAL_ADDRESS 65535

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr,
                "Uso: ./vm <arquivo_enderecos> <fifo|lru> <fifo|lru>\n");
        return EXIT_FAILURE;
    }

    /* Identifica os algoritmos escolhidos. */
    int page_lru = strcmp(argv[2], "lru") == 0;
    int tlb_lru = strcmp(argv[3], "lru") == 0;

    if ((!page_lru && strcmp(argv[2], "fifo") != 0) ||
        (!tlb_lru && strcmp(argv[3], "fifo") != 0)) {
        fprintf(stderr, "Algoritmo inválido. Use fifo ou lru.\n");
        return EXIT_FAILURE;
    }

    /* Abre os arquivos. */
    FILE *address_file = fopen(argv[1], "r");

    if (address_file == NULL) {
        perror("Erro ao abrir arquivo de endereços");
        return EXIT_FAILURE;
    }

    FILE *backing_store = fopen("BACKING_STORE.bin", "rb");

    if (backing_store == NULL) {
        perror("Erro ao abrir BACKING_STORE.bin");
        fclose(address_file);
        return EXIT_FAILURE;
    }

    FILE *output_file = fopen("correct.txt", "w");

    if (output_file == NULL) {
        perror("Erro ao criar correct.txt");
        fclose(address_file);
        fclose(backing_store);
        return EXIT_FAILURE;
    }

    /* Inicializa as estruturas. */
    PhysicalMemory memory;
    PageTable page_table;
    FIFOReplacement fifo;
    LRUReplacement lru;
    TLB tlb;

    initialize_memory(&memory);
    initialize_page_table(&page_table);
    initialize_fifo(&fifo);
    initialize_lru(&lru);
    initialize_tlb(&tlb);

    /* Estatísticas. */
    unsigned long total_addresses = 0;
    unsigned long tlb_hits = 0;
    unsigned long tlb_misses = 0;
    unsigned long page_faults = 0;

    unsigned long virtual_address;

    /* Processa os endereços virtuais. */
    while (1) {
        int result = fscanf(address_file, "%lu", &virtual_address);

        if (result == EOF) {
            break;
        }

        if (result != 1 || virtual_address > MAX_VIRTUAL_ADDRESS) {
            fprintf(stderr, "Erro: endereço virtual inválido.\n");

            fclose(address_file);
            fclose(backing_store);
            fclose(output_file);

            return EXIT_FAILURE;
        }

        total_addresses++;

        Address address = decode_address((unsigned int)virtual_address);

        /* Primeiro procura a página na TLB. */
        int frame_number = search_tlb(
            &tlb,
            address.page_number,
            tlb_lru
        );

        int tlb_hit = (frame_number != -1);

        if (tlb_hit) {
            tlb_hits++;

            /* Atualiza o LRU da memória física, se necessário. */
            if (page_lru) {
                lru_register_frame(&lru, frame_number);
            }
        } else {
            tlb_misses++;

            /* Verifica se a página já está na tabela de páginas. */
            int page_was_valid =
                page_table.entries[address.page_number].valid;

            /* Busca ou carrega a página na memória física. */
            frame_number = load_virtual_page(
                backing_store,
                address.page_number,
                &memory,
                &page_table,
                &fifo,
                &lru,
                page_lru
            );

            if (frame_number == -1) {
                fprintf(stderr, "Erro ao carregar página virtual.\n");

                fclose(address_file);
                fclose(backing_store);
                fclose(output_file);

                return EXIT_FAILURE;
            }

            if (!page_was_valid) {
                page_faults++;
            }

            /* Remove traduções antigas do frame reutilizado. */
            invalidate_tlb_frame(&tlb, frame_number);

            /* Insere a nova tradução na TLB. */
            insert_tlb(
                &tlb,
                address.page_number,
                frame_number,
                tlb_lru
            );
        }

        /* Calcula o endereço físico. */
        int physical_address =
            frame_number * PAGE_SIZE + address.offset;

        /* Obtém o valor armazenado. */
        int value = memory.bytes[physical_address];

        /* Escreve o resultado no arquivo. */
        fprintf(
            output_file,
            "Virtual address: %u TLB: %d Physical address: %d Value: %d\n",
            address.virtual_address,
            tlb_hit,
            physical_address,
            value
        );
    }

    if (ferror(address_file)) {
        fprintf(stderr, "Erro durante a leitura dos endereços.\n");

        fclose(address_file);
        fclose(backing_store);
        fclose(output_file);

        return EXIT_FAILURE;
    }

    /* Calcula as taxas. */
    double tlb_hit_rate = 0.0;
    double page_fault_rate = 0.0;

    if (total_addresses > 0) {
        tlb_hit_rate =
            (double)tlb_hits / total_addresses * 100.0;

        page_fault_rate =
            (double)page_faults / total_addresses * 100.0;
    }

    /* Exibe as estatísticas. */
    printf("\nVirtual Memory Manager\n");
    printf("Total de endereços: %lu\n", total_addresses);
    printf("TLB hits: %lu\n", tlb_hits);
    printf("TLB misses: %lu\n", tlb_misses);
    printf("Taxa de acerto da TLB: %.2f%%\n", tlb_hit_rate);
    printf("Page faults: %lu\n", page_faults);
    printf("Taxa de page faults: %.2f%%\n", page_fault_rate);
    printf("Resultados salvos em correct.txt\n");

    fclose(address_file);
    fclose(backing_store);
    fclose(output_file);

    return EXIT_SUCCESS;
}
