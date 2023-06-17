#ifndef PATTERN_IMPL
#define PATTERN_IMPL
#include <stdlib.h>
#include <stdbool.h>
typedef int pattern_symbol;

typedef struct pattern_node {
  pattern_symbol p;
  struct pattern_node *next;
} pattern_node;

typedef struct pattern {
  pattern_node *begin;
} pattern;

pattern_node *pat_node_create(pattern_symbol p);
void pat_node_delete(pattern_node *p_n);
pattern *pattern_create(pattern_symbol p);
void pattern_delete(pattern *p);
bool pattern_append_sym(pattern *p, pattern_symbol p_s);
bool pattern_remove_sym(pattern *p, pattern_symbol p_s);
#endif
