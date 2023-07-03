#ifndef ERROR_HANDLING
#define ERROR_HANDLING
#include <stdio.h>
#include <stdlib.h>

#define ERROR_TRIE 1
#define ERROR_PATTERN 2
#define ERROR_LZ78 3
#define DEBUG_TRIE 1
#define DEBUG_PATTERN 2
#define DEBUG_LZ78 3
void handle_error(int i, char *str);
void handle_debug(int i, char *str);
#endif // !ERROR_HANDLING
