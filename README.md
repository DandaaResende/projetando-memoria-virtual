# Projetando uma Memória Virtual

Implementação em C de um simulador de gerenciamento de memória virtual, baseada no exercício *Designing a Virtual Memory Manager*, apresentado na página P-51 do livro *Operating System Concepts*, 10ª edição.

## Objetivo

Simular a tradução de endereços virtuais em endereços físicos, utilizando uma tabela de páginas e uma Translation Lookaside Buffer (TLB).

A implementação considera:

- Memória física com 128 frames;
- Páginas de 256 bytes;
- Espaço de endereçamento virtual de 16 bits;
- Tabela de páginas com 256 entradas;
- Substituição de páginas utilizando FIFO ou LRU;
- Substituição de entradas da TLB utilizando FIFO ou LRU;
- Busca concorrente na TLB, com uma thread para cada entrada.

## Tecnologias utilizadas

- Linguagem C;
- POSIX Threads (pthreads);
- GCC;
- GNU Make;
- Linux, Unix ou macOS.

## Organização do projeto

```text
dmsrb/
├── include/
│   ├── address.h
│   ├── backing_store.h
│   ├── memory.h
│   ├── page_table.h
│   ├── replacement.h
│   ├── tlb.h
│   └── vm_manager.h
├── src/
│   ├── address.c
│   ├── backing_store.c
│   ├── main.c
│   ├── memory.c
│   ├── page_table.c
│   ├── replacement.c
│   ├── tlb.c
│   └── vm_manager.c
├── Makefile
└── README.md
```

## Compilação

Para compilar o programa, execute na raiz do projeto:

```bash
make
```

O comando gera o executável `vm`.

Para remover os arquivos gerados durante a compilação e o arquivo de saída:

```bash
make clean
```

## Execução

O programa recebe três argumentos:

1. Arquivo contendo os endereços virtuais;
2. Algoritmo de substituição de páginas (`fifo` ou `lru`);
3. Algoritmo de substituição da TLB (`fifo` ou `lru`).

Exemplo:

```bash
./vm addresses.txt lru fifo
```

Nesse exemplo, FIFO não é utilizado para a memória: o algoritmo de substituição de páginas é LRU, enquanto o algoritmo de substituição da TLB é FIFO.

O arquivo de endereços e o arquivo `BACKING_STORE.bin`, fornecidos pelo professor, devem estar disponíveis no diretório de execução.

## Arquivo de saída

Após a execução, o programa gera o arquivo `correct.txt`, contendo os endereços virtuais processados, a indicação de acerto ou falta na TLB, os endereços físicos correspondentes e os valores armazenados na memória.

Ao final, são apresentadas estatísticas de execução:

- Total de endereços processados;
- Acertos e faltas da TLB;
- Taxa de acerto da TLB;
- Quantidade de page faults;
- Taxa de page faults.

## Tratamento de erros

O programa verifica os argumentos recebidos, os algoritmos informados e a abertura dos arquivos necessários. Também verifica se os endereços fornecidos são válidos e se a leitura dos arquivos ocorre corretamente.

Em caso de erro, uma mensagem explicativa é apresentada e a execução é encerrada.

## Autoria

Projeto desenvolvido como atividade acadêmica da disciplina de Sistemas Operacionais.
