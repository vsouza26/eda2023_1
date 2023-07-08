#ifndef LZ78
#define LZ78
#include "./trie/trie.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <bits/getopt_core.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <endian.h>
#include <math.h>
#define ACTIVATE_LZ78_DEBUG 0
typedef unsigned char byte;

typedef struct uncompress_output {
  unsigned int index;
  bool bit;
} uncompress_output;

typedef u_int64_t compressed_output;

void lz78_compress(char *caminho, char *saida);
void lz78_expand(char *caminho, char *saida);
bool get_bit(byte b, int pos);
compressed_output compress_index_and_bit(unsigned int index, bool bit); 
uncompress_output uncompress_output_index_and_bit(compressed_output co); 

#endif
