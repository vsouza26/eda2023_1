#ifndef TRIE_IMPL
#define TRIE_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
typedef unsigned char bit;

typedef struct trie_node{
  trie_node *zero;
  trie_node *one;
  bit b;
} trie_node;

typedef struct trie{
  trie_node *head;
} trie;

trie *trie_create_tree();
void trie_add_child(trie_node* node);
void trie_rem_child(trie_node* node);
bool trie_exists(trie_node* node, pattern *p); 
#endif // !TRIE_IMPL TRIE_IMPL
