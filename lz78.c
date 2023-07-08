#include "./lz78.h"
#include "error_handling/error_handling.h"
#include "trie/pattern.h"
#include "trie/trie.h"
#include <endian.h>
#include <stdio.h>
#include <sys/types.h>

char buffer;
int buffer_size = 0;
unsigned int BIT_REPR = 1;
unsigned int BIT_BUFFER_REPR = 0;

void force_buffer_flush(FILE * f_pointer, uncompress_output uco){
    for (int i = 0; i < (buffer_size - 8) * -1; i++){
      buffer = buffer << 1;
      buffer += 1;
    }
    fwrite(&buffer, 1, sizeof(char),f_pointer); 
}

void add_bit_to_buffer(pattern_symbol p_s, FILE* f_pointer){
  buffer = buffer << 1;
  buffer += p_s;
  buffer_size += 1;
  if(ACTIVATE_LZ78_DEBUG)
    printf("Adicionando %d ao buffer\n", p_s);
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

bool get_bit_ftb(u_int32_t b, int position){
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
    trie_node *t_n = t->head;
    trie_node *t_n_aux = t->head;
    //CRIAR FLAG DE FINAL DE ARQUIVO
    uncompress_output final_pattern;
    bool flag_first_pattern = false;
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
        t_n = t_n->filhos[bit];
        if(!t_n){
          int max_index = trie_add_symbol(t, t_n_aux, bit);
          if(!flag_first_pattern){
            final_pattern.index = 0;
            final_pattern.bit = bit;
            flag_first_pattern = true;
          }
          if(ACTIVATE_LZ78_DEBUG)
            printf("\ndict_pos: %d\nbit_repr: %d\n", t_n_aux->dict_pos, BIT_REPR);
          for(int i = BIT_REPR - 1; i >= 0 ; i--) {
            pattern_symbol co_bit = get_bit_ftb(t_n_aux->dict_pos, i);
            add_bit_to_buffer(co_bit, compressed);
          } 
          add_bit_to_buffer(bit, compressed);
          t_n = t->head;
          t_n_aux = t->head;
          if(max_index > 1)
            BIT_REPR = (int) floor(log2(t->dict)) + 1;
        } else {
          t_n_aux = t_n;
        }
      }
    }
    fclose(to_compress);
    //adiciona o padrão final no fim do arquivo
    if(ACTIVATE_LZ78_DEBUG)
      printf("padrão de fim de arquivo a seguir\n");
    for(int i = BIT_REPR - 1; i >= 0 ; i--) {
      pattern_symbol co_bit = get_bit_ftb(final_pattern.index, i);
      add_bit_to_buffer(co_bit, compressed);
    } 
    add_bit_to_buffer(final_pattern.bit, compressed);
    if(buffer_size){
        force_buffer_flush(compressed, final_pattern);
    }
    fclose(compressed);
}

void lz78_expand(char *caminho, char *saida){
    FILE *to_expand = fopen(caminho, "r+b");
    FILE *expanded = fopen(saida, "w+b");
    char c; 
    trie *t = trie_create();
    unsigned int num_bits_a_ler = BIT_REPR + 1;
    unsigned int num_bits_lidos = 0;
    //É preciso criar um outro buffer, um de entrada que consegue ler os arquivos e outro de saida.
    uncompress_output final_pattern;
    bool final_pattern_flag = false;
    u_int32_t buffer_entry = 0x00;
    while(fread(&c, sizeof(char), 1,to_expand) == 1){
      for (int i = 0; i < 8; i++){
        bool bit = get_bit(c, i);
        buffer_entry = buffer_entry << 1;
        buffer_entry += bit;
        if(ACTIVATE_LZ78_DEBUG){
          printf("Buffer_entry:");
          for(int i = 0; i < 8; i++){
            printf("%d", get_bit(buffer_entry, i));
          }
          printf("\n");
        }
        num_bits_lidos++;
        if(num_bits_lidos == num_bits_a_ler){
          uncompress_output uco = uncompress_output_index_and_bit(buffer_entry);
          if(ACTIVATE_LZ78_DEBUG){
            printf("index: %d\n", uco.index);
          }
          if(ACTIVATE_LZ78_DEBUG){
            printf("final_pattern.index: %d, final_pattern.bit: %d\nuco.index: %d, uco.bit: %d\n", final_pattern.index, final_pattern.bit, uco.index, uco.bit);
          }
          if(uco.bit == final_pattern.bit && uco.index == final_pattern.index && final_pattern_flag ==  true){
             exit(0);
          }
          if(uco.index){
            pattern *output_p = trie_retrieve_pattern(t->array[uco.index]);
            pattern_node *p_n = output_p->begin;
            while(p_n){
              if(ACTIVATE_LZ78_DEBUG)
                printf("Adding %d from pattern\n", p_n->p);
              add_bit_to_buffer(p_n->p, expanded);
              p_n = p_n->next;
            }
            int max_index = trie_add_symbol(t, t->array[uco.index], bit);
            pattern_delete(&output_p);
          }
          if(!uco.index){
            if(!final_pattern_flag){
              final_pattern.index = 0;
              final_pattern.bit = bit;
              final_pattern_flag = true;
            }
            trie_add_symbol(t, t->head, bit);
          }
          add_bit_to_buffer(uco.bit, expanded);
          buffer_entry = 0x00;
          num_bits_lidos = 0;
          if(t->dict > 2)
            BIT_REPR = floor(log2(t->dict)) + 1;
          num_bits_a_ler = BIT_REPR + 1;
          if(ACTIVATE_LZ78_DEBUG)
            printf("Fim da palavra a ser lida\nBIT_REPR:%d\nBIT_BUFFER_REPR:%d\nnum_bits_a_ler:%d\nt->dict:%d\n", BIT_REPR, BIT_BUFFER_REPR, num_bits_a_ler,t->dict);
        }
      }
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
