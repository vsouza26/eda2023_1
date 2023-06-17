#include "pattern.h"

pattern_node *pat_node_create(pattern_symbol p){
  pattern_node *pat_node = malloc(sizeof(pattern_node));
  if(pat_node == NULL){
    return NULL;
  }
  pat_node->p = p;
  pat_node->next = NULL;
  return pat_node;
};

void pat_node_delete(pattern_node *p_n){
  free(p_n);
}

pattern *pattern_create(pattern_symbol p){
  pattern *pat = malloc(sizeof(pattern));
  if (pat == NULL){
    return NULL;
  }
  pat->begin = pat_node_create(p);
  return pat;
}

void pattern_delete(pattern *p){
  pattern_node * p_n = p->begin;
  while(p_n != NULL){
    pattern_node *next_node = p_n->next;
    pat_node_delete(p_n);
    p_n = next_node;
  }
  free(p);
}

bool pattern_append_sym(pattern *p, pattern_symbol p_s){
  pattern_node *p_n = p->begin;
  pattern_node *p_n_append;
  pattern_node *node_to_add = pat_node_create(p_s);
  if (!node_to_add){
    return false;
  }
  while(p_n != NULL){
    p_n_append = p_n;
    p_n = p_n->next;
  }
  p_n_append->next = node_to_add;
  return true;
};

bool pattern_remove_sym(pattern *p, pattern_symbol p_s){
  pattern_node *p_n = p->begin;
  pattern_node *p_n_append;
  pattern_node *aux;
  while(p_n->p != p_s){
    p_n = p_n->next;
    if (p_n == NULL){
      return false;
    }
  }
  p_n_append->next
  return true;
};
