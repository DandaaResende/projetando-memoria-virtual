CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pthread -Idmsrb/include

SRC = dmsrb/src/main.c \
      dmsrb/src/address.c \
      dmsrb/src/memory.c \
      dmsrb/src/page_table.c \
      dmsrb/src/backing_store.c \
      dmsrb/src/vm_manager.c \
      dmsrb/src/replacement.c \
      dmsrb/src/tlb.c

OBJ = $(SRC:.c=.o)

all: vm

vm: $(OBJ)
	$(CC) $(CFLAGS) -o vm $(OBJ)

clean:
	rm -f $(OBJ) vm correct.txt

.PHONY: all clean
