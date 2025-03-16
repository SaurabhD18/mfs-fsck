CC = gcc
CFLAGS = -Wall -Iinclude

all: mkfs

mkfs: src/mkfs.o src/ext2_helpers.o
	$(CC) $(CFLAGS) -o mkfs src/mkfs.o src/ext2_helpers.o

src/mkfs.o: src/mkfs.c include/ext2_defs.h src/ext2_helpers.h
	$(CC) $(CFLAGS) -c src/mkfs.c -o src/mkfs.o

src/ext2_helpers.o: src/ext2_helpers.c src/ext2_helpers.h
	$(CC) $(CFLAGS) -c src/ext2_helpers.c -o src/ext2_helpers.o

clean:
	rm -f src/*.o mkfs

