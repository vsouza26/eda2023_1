#include "error_handling.h"


void handle_error(int i, char *str){
  switch (i) {
    case ERROR_TRIE:{
            printf("[Trie] %s\n", str);
            exit(101);
            break;
           }
    case ERROR_PATTERN:{
            printf("[Pattern] %s\n", str);
            exit(102);
            break;
           }
    case ERROR_LZ78:{
            printf("[LZ78] %s\n", str);
            exit(103);
            break;
           }
    default:{
            printf("Tratameto de erro mal sucedido");
             }
  }
}

void handle_debug(int i, char *str){
  switch (i) {
    case DEBUG_TRIE:{
            fprintf(stderr, "[Trie] %s\n", str);
            break;
           }
    case DEBUG_PATTERN:{
            fprintf(stderr, "[Pattern] %s\n", str);
            break;
           }
    case DEBUG_LZ78:{
            fprintf(stderr, "[LZ78] %s\n", str);
            break;
           }
    default:{
            printf("Tratameto de erro mal sucedido");
             }
  }
}
