TARGET=lz78_compressor
CXX=gcc
LD=gcc
ARGS=-lm
OBJS=trie.o pattern.o error_handling.o lz78.o
all:$(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(ARGS)
lz78.o: ./lz78.c
	$(CC) -c ./lz78.c -o lz78.o -lm
trie.o: ./trie/trie.c
	$(CC) -c ./trie/trie.c -o trie.o
pattern.o: ./trie/pattern.c
	$(CC) -c ./trie/pattern.c -o pattern.o
error_handling.o: ./error_handling/error_handling.c
	$(CC) -c ./error_handling/error_handling.c -o error_handling.o
clear:
	rm -f *.o
