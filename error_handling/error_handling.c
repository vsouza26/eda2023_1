#include "error_handling.h"


void handle_error(int i, char *str){
  switch (i) {
    case ERROR_TRIE:{
            printf("[Trie] %s", str);
            break;
           }
    case ERROR_PATTERN:{
            printf("[Pattern] %s", str);
            break;
           }
    case ERROR_LZ78:{
            printf("[LZ78] %s", str);
            break;
           }
    default:{
            printf("Tratameto de erro mal sucedido");
             }
  }
}
