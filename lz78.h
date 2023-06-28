#ifndef LZ78
#define LZ78
#include "./trie/trie.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define DICT_SIZE 256
typedef unsigned char byte;

void lz78_compress(char *caminho, char *saida);
bool get_bit(byte b, int pos);
#endif
