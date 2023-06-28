#include "./lz78.h"
#include "error_handling/error_handling.h"
#include "trie/pattern.h"
#include "trie/trie.h"
#include <bits/getopt_core.h>
#include <stdio.h>
#include <stdlib.h>

bool get_bit(byte b, int position){
  if(position>7) {
    handle_error(ERROR_LZ78, "Posição de bit inexistente");
    exit(1);
  }
  position = position - 7; 
  position = position * -1;
  return (b >> position) & 0x1;
}

void lz78_compress(char *caminho, char *saida){
  FILE *to_compress = fopen(caminho, "r+b");
  char c; 
  trie_node *dict_pattern[DICT_SIZE];
  int dict_pattern_cursor = 0;
  trie *t = trie_create();
  pattern *p = pattern_create();
  while((c = fgetc(to_compress))){
    if(feof(to_compress)) break;
    for(int i = 0; i < 8; i++){
      for(int j = i; j < 8; j++){
        bool bit = get_bit(c, j);
        printf("%d",bit); 
      }
      printf("\n");
      bool bit = get_bit(c, i);
      bit = (pattern_symbol) bit;
      pattern_append(p, bit);
      pattern_list(p);
      if(!trie_exists_pattern(t,p)){
        printf("Padrão não existe\n");
        dict_pattern[dict_pattern_cursor] = trie_add_pattern_return_last_elem_att_dict_pos(t, p, dict_pattern_cursor);
        dict_pattern_cursor++;
        p = pattern_create();
      }
    }
    printf("\n");
  }
  trie_list(t->head);
}

int main(int argc, char *argv[]){
  int opt;
  char caminho[100];
  if(argc < 2){ fprintf(stderr, "Usage: lz78_compress <path_to_filho>\n"); return 1; }
  while ((opt = getopt(argc, argv, "i:h")) != -1){
    switch(opt){
      case 'i':{
        strcpy(caminho, optarg);
        printf("caminho: %s\n", caminho);
        break;
      }
      case 'h':{
        fprintf(stderr, "Usage: lz78_compress <path_to_filho>\n");
        break;
      }
      case '?':{
        if (optopt == 'i')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        return 1;
      }
      default:{ abort();}
    }
  }
  lz78_compress(caminho, "teste");
}
