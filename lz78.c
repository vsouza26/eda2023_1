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
    if(ACTIVATE_LZ78_DEBUG)
      handle_debug(DEBUG_LZ78, "buffer cheio, escrevendo no arquivo\n");
  }
}

uncompress_output uncompress_output_index_and_bit(compressed_output co){
  uncompress_output uco;
  uco.bit = co%2;
  uco.index = co >> 1;
  return uco;
}

compressed_output compress_index_and_bit(unsigned int index, bool bit){
  compressed_output output = index << 1;
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
    if(to_compress == NULL)
      return handle_error(ERROR_LZ78, "Arquivo não existente");
    FILE *compressed = fopen(saida, "w+b");
    if(compressed == NULL)
     return handle_error(ERROR_LZ78, "Não pode criar arquivo para saída");
    char c; 
    int dict_pattern_cursor = 0;
    trie *t = trie_create();
    pattern *p = pattern_create();
    while(fread(&c, sizeof(char), 1, to_compress) == 1){
      for(int i = 0; i < 8; i++){
        if(ACTIVATE_LZ78_DEBUG){
          for(int j = i; j < 8; j++){
            bool bit = get_bit(c, j);
            printf("%d",bit);
          }
          printf("\n");
        }
        bool bit = get_bit(c, i);
        bit = (pattern_symbol) bit;
        pattern_append(p, bit);
        if(!trie_exists_pattern(t,p)){
          trie_add_pattern(t, p);
          pattern_pop(&p);
          unsigned int index = trie_get_index_of_pattern(t,p);
          compressed_output co = compress_index_and_bit(index,bit);
          fwrite(&co, 1, sizeof(compressed_output), compressed);
          if(p != NULL)
            pattern_delete(&p);
          p = pattern_create();
        }
        //trie_list(t->head);
      }
    }
    fclose(to_compress);
    if(p->begin != NULL){
          pattern_symbol p_s = pattern_pop(&p);
          unsigned int index = trie_get_index_of_pattern(t,p);
          compressed_output co = compress_index_and_bit(index,p_s);
          fwrite(&co, 1, sizeof(compressed_output), compressed);
    }
  }

void lz78_expand(char *caminho, char *saida){
    FILE *to_expand = fopen(caminho, "r+b");
    FILE *expanded = fopen(saida, "w+b");
    compressed_output co; 
    trie *t = trie_create();
    while(fread(&co, sizeof(compressed_output), 1,to_expand) == 1){
      uncompress_output uco = uncompress_output_index_and_bit(co);
      if(uco.index){
        pattern *output_p = trie_retrieve_pattern(t->array[uco.index]);
        pattern_node *p_n = output_p->begin;
        while(p_n){
          add_bit_to_buffer(p_n->p, expanded);
          p_n = p_n->next;
        }
        pattern_append(output_p, uco.bit);
        trie_add_pattern(t, output_p);
        pattern_delete(&output_p);
      }
      if(!uco.index){
        pattern *p = pattern_create();
        pattern_append(p, uco.bit);
        trie_add_pattern(t, p);
        pattern_delete(&p);
      }
      add_bit_to_buffer(uco.bit, expanded);
    }
    fclose(to_expand);
    fclose(expanded);
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
}
