
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pthread -Iinclude

SRC = src/main.c src/address.c src/memory.c src/page_table.c src/backing_store.c
OBJ = $(SRC:.c=.o)

all: vm

vm: $(OBJ)
	$(CC) $(CFLAGS) -o vm $(OBJ)

clean:
	rm -f $(OBJ) vm correct.txt

.PHONY: all clean
