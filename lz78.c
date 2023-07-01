#include "./lz78.h"
#include "error_handling/error_handling.h"
#include "trie/pattern.h"
#include "trie/trie.h"
#include <endian.h>
#include <stdio.h>
#include <sys/types.h>

char buffer;
int buffer_size = 0;


void add_bit_to_buffer(pattern_symbol p_s, FILE* f_pointer){
  buffer = buffer << 1;
  buffer += p_s;
  buffer_size += 1;
  if (buffer_size == 8){
    fwrite(&buffer, 1, sizeof(char),f_pointer); 
    buffer_size = 0;
    printf("buffer cheio, escrevendo no arquivo\n");
  }
}

uncompress_output uncompress_output_index_and_bit(compressed_output co){
  uncompress_output uco;
  uco.bit = co%2;
  uco.index = co >> 1;
  return uco;
}

compressed_output compress_index_and_bit(unsigned int index, bool bit){
  u_int32_t output = index << 1;
  output += bit;
  return output;
}

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
    FILE *compressed = fopen(saida, "w+b");
    char c; 
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
          trie_add_pattern(t, p);
          pattern_pop(&p);
          unsigned int index = trie_get_index_of_pattern(t,p);
          compressed_output co = compress_index_and_bit(index,bit);
          fwrite(&co, 1, sizeof(compressed_output), compressed);
          pattern_delete(&p);
          p = pattern_create();
        }
      }
      printf("\n");
    }
    if(p->begin != NULL){
          pattern_symbol p_s = pattern_pop(&p);
          unsigned int index = trie_get_index_of_pattern(t,p);
          compressed_output co = compress_index_and_bit(index,p_s);
          fwrite(&co, 1, sizeof(compressed_output), compressed);
    }
    trie_list(t->head);
  }

void lz78_expand(char *caminho, char *saida){
    FILE *to_expand = fopen(caminho, "r+b");
    FILE *expanded = fopen(saida, "w+b");
    compressed_output co; 
    trie *t = trie_create();
    pattern *p = pattern_create();
    while((fread(&co, 1, 4,to_expand))){
      if(feof(to_expand)) break;
      uncompress_output uco = uncompress_output_index_and_bit(co);
      printf("uco.index: %d, uco.bit: %d",uco.index, uco.bit);
      if(uco.index){
        if(uco.index > t->dict)
          printf("Warning!!! Garbage ahead");
        pattern *output_p = trie_retrieve_pattern(t->array[uco.index]);
        pattern_node *p_n = output_p->begin;
        printf("\npattern start:\n");
        while(p_n){
          add_bit_to_buffer(p_n->p, expanded);
          printf(" %d ", p_n->p);
          p_n = p_n->next;
        }
        printf("\npattern finish\n");
        pattern_append(output_p, uco.bit);
        trie_add_pattern(t, output_p);
      }
      if(!uco.index){
        pattern_append(p, uco.bit);
        trie_add_pattern(t, p);
      }
      trie_list(t->head);
      printf("adding %d to buffer\n", uco.bit);
      add_bit_to_buffer(uco.bit, expanded);
      pattern_delete(&p);
      p = pattern_create();
    }
    for(int i = 1; i < 3; i++){
      printf("t array index: %d, symbol: %d\n", i,t->array[i]->p_s);
    }
}

  int main(int argc, char *argv[]){
    int opt;
    bool flag_i = false, flag_o = false, flag_c = false, flag_e = false;
    char caminho[100];
    char caminho_saida[100];
    if(argc < 2){ fprintf(stderr, "Usage: lz78_compress <path_to_filho>\n"); return 1; }
    while ((opt = getopt(argc, argv, "i:ho:ec")) != -1){
      switch(opt){
        case 'i':{
          flag_i = true;
          strcpy(caminho, optarg);
          break;
        }
        case 'h':{
          fprintf(stderr, "Usage: lz78_compress -i <path_to_input> -o <output_file>\n");
          break;
        }
        case 'o':{
          flag_o = true;
          strcpy(caminho_saida, optarg);
          break;
        }
        case 'c':{
          flag_c = true;
          break;
        }
        case 'e':{
          flag_e = true;
          break;
        }
        case '?':{
          if (optopt == 'i')
            fprintf (stderr, "Option -%c requires an argument.\n", optopt);
          if (optopt == 'o')
            fprintf (stderr, "Option -%c requires an argument.\n", optopt);
          return 1;
        }
        default:{ abort();}
      }
    }
    if((!flag_i) || (!flag_o)){
         fprintf(stderr, "Usage: lz78_compress [-d,-c] -i <path_to_input> -o <output_file>\n");
         exit(1);
    }
    if((!flag_c) && (!flag_e)){
         fprintf(stderr, "Usage: lz78_compress [-d,-c] -i <path_to_input> -o <output_file>\n");
         exit(1);
    }
    if(flag_c && flag_e){
         fprintf(stderr, "You can't compress and expand files at the same time\n");
         exit(1);
    }
    if(flag_c){
      lz78_compress(caminho, caminho_saida);
    }
    if(flag_e){
      lz78_expand(caminho, caminho_saida);
    }
    FILE *endianess = fopen("endianess", "w+b");
    int r = 257;
    fwrite(&r,1, sizeof(int), endianess);
    fclose(endianess);
}
