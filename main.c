#include <stdio.h>
#include <stdlib.h>
  
int main(int argc, char **argv) {
  if (argc != 3){
    fprintf(stderr, "Usage: compressor <arquivo> <saida>\n");
    exit(1);
  }
  FILE *f_in = fopen(argv[1],"rb+");
  FILE *f_out = fopen(argv[2],"rb+");
}
