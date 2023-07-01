#ifndef TRIE_IMPL
#define TRIE_IMPL

#define ADDR_AVAILABE 100000
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pattern.h"
#include "../error_handling/error_handling.h"

typedef struct trie_node{
  pattern_symbol p_s;
  struct trie_node *filhos[GRAMMAR_SIZE];
  struct trie_node *parent;
  bool end;
  unsigned int dict_pos;
} trie_node;

typedef struct trie{
  trie_node *head;
  unsigned int dict;
  //Endereços compostos por 31 bits
  trie_node *array[ADDR_AVAILABE];
} trie;

trie *trie_create();
void trie_add_pattern(trie *t, pattern *p);
trie_node *trie_add_pattern_return_last_elem_att_dict_pos(trie *t, pattern *p, int pos);
trie_node *_trie_create_node(pattern_symbol ps);
void trie_rem_child(trie_node* node);
bool trie_exists_pattern(trie *t, pattern *p); 
void trie_list(trie_node *t_n);
pattern *trie_retrieve_pattern(trie_node *t_n);
int trie_get_index_of_pattern(trie *t, pattern *p);
#endif // !TRIE_IMPL TRIE_IMPL
