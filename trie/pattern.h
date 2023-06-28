#ifndef PATTERN_IMPL
#define PATTERN_IMPL
#define GRAMMAR_SIZE 2
#define NO_SYMBOL 256
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
typedef int pattern_symbol;

typedef struct pattern_node {
  pattern_symbol p;
  struct pattern_node *next;
} pattern_node;

typedef struct pattern {
  pattern_node *begin;
} pattern;

pattern_node *pat_node_create(pattern_symbol p);
void pat_node_delete(pattern_node **p_n);
pattern *pattern_create();
void pattern_delete(pattern **p_ref);
bool pattern_is_valid(pattern *p);
bool pattern_append(pattern *p, pattern_symbol p_s);
//remove primeira ocorrencia do simbolo no padrao
pattern_symbol pattern_pop(pattern **p_ref);
void pattern_trim(pattern **p_ref, int num);
void pattern_list(pattern *p);
bool pattern_equals(pattern *p1, pattern *p2);
void pattern_shove(pattern *p, pattern_symbol p_s);
#endif
