#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define DICT_SIZE 256

typedef unsigned char byte;
typedef byte pat_repr;
typedef byte bit;

typedef struct dict_tuple {
  pat_repr pat;
  int size;
} pat_tuple;

bool getBit(byte b, int position){
  return (b >> position) & 0x1;
}

byte concat_pat(pat_tuple pat, bit b){
  return (pat.pat << 1)  & b; 
}

bool lz78_compresser(char *f_in_str, char *f_out_str){
  pat_tuple *dict[DICT_SIZE];
  //initiate nulled array:
  for (int i = 0; i < DICT_SIZE; i++){
    dict[i] = 0;
  } 
  FILE *f_in = fopen(f_in_str, "r+b");
  byte c;
  pat_tuple pat_aux;
  while ((c = fgetc(f_in)) > 0){
    //RECEBE BYTE DE ARQUIVO (NÃO É POSSÍVEL LER BIT POR BIT)
    for (int i = sizeof(byte) - 1; i >= 0; i--){
      //PROCESSA BIT POR BIT DO ARQUIVO.
      bit k = getBit(c, i);
    }
  }

};

int main(){
  lz78_compresser("./teste", "alo");
}
