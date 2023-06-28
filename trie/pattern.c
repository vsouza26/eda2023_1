#include "pattern.h"
#include "../error_handling/error_handling.h"

bool pattern_is_valid(pattern *p){
  if (p != NULL) return true;
  else {
    handle_error(ERROR_PATTERN, "Padrão não válido");
    return false;
  }
}

pattern_node *pat_node_create(pattern_symbol p){
  pattern_node *pat_node = malloc(sizeof(pattern_node));
  if(pat_node == NULL){
    handle_error(ERROR_PATTERN, "Erro na alocação de um nó");
    return NULL;
  }
  pat_node->p = p;
  pat_node->next = NULL;
  return pat_node;
};

void pat_node_delete(pattern_node **p_n){
  if(p_n != NULL){
    free(p_n);
    *p_n = NULL;
  }
  else
    handle_error(ERROR_PATTERN, "Tentativa deletar nó com ponteiro NULO");
}

pattern *pattern_create(){
  pattern *pat = malloc(sizeof(pattern));
  if (pat == NULL){
    handle_error(ERROR_PATTERN, "Erro na alocação de padrão");
    return NULL;
  }
  pat->begin = NULL;
  return pat;
}

void pattern_delete(pattern **p_ref){
  pattern *p = *p_ref;
  if(!pattern_is_valid(p)) return handle_error(ERROR_PATTERN, "Tentativa de deletar um padrão não válido");
  pattern_node *p_n = p->begin;
  while(p_n != NULL){
    pattern_node *next_node = p_n->next;
    pat_node_delete(&p_n);
    p_n = next_node;
  }
  free(p);
  *p_ref = NULL;
}

bool pattern_append(pattern *p, pattern_symbol p_s){
  if(!pattern_is_valid(p)){ handle_error(ERROR_PATTERN, "Tentativa acrescentar simbolo em padrão não válido"); return false;}
  if (p->begin == NULL){
    pattern_node *node = pat_node_create(p_s);
    p->begin = node;
    return true;
  }
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
  if(!pattern_is_valid(p)){ handle_error(ERROR_PATTERN, "Tentativa remover simbolo em padrão não válido"); return false;}
  pattern_node *p_n = p->begin;
  pattern_node *aux;
  while(p_n->p != p_s){
    if (p_n == NULL){
      return false;
    }
    aux = p_n;
    p_n = p_n->next;
  }
  if(p_n == p->begin){
    p->begin = p_n->next;
    pat_node_delete(&p_n);
    return true;
  } else {
    aux->next = p_n->next;
    pat_node_delete(&p_n);
    return true;
  }
};

void pattern_list(pattern *p){
  if(!pattern_is_valid(p)) { handle_error(ERROR_PATTERN, "Tentativa listar padrão não válido");}
  pattern_node *p_n = p->begin;
  printf("Pattern Begin:");
  while (p_n){
    printf("%d->", p_n->p);
    p_n = p_n->next;
  }
  printf("NULL\n");
  return;
}

pattern_symbol pattern_pop(pattern **p_ref){
  pattern *p = *p_ref;
  if(!pattern_is_valid(p)) {  handle_error(ERROR_PATTERN, "Tentativa retirar item de padrão não válido"); return NO_SYMBOL; }
  pattern_node *p_n = p->begin;
  pattern_node *aux = NULL;
  while(p_n->next){
    aux = p_n;
    p_n = p_n->next;
  }
  if (aux){
    aux->next = NULL;
    return p_n->p;
  } else {
    p->begin = NULL;
    pattern_delete(p_ref);
    return p_n->p;
  }
}

void pattern_trim(pattern **p_ref, int num){
  pattern *p = *p_ref;
  if(!pattern_is_valid(p)) handle_error(ERROR_PATTERN, "Tentativa de cortar de padrão não válido"); return;
  pattern_node *p_n = p->begin;
  int numero = num - 1;
  pattern_node *aux = NULL;
  if (!p_n){
    handle_error(ERROR_PATTERN, "Tentando cortar de um padrão sem simbolos");
    return;
  }
  while (numero > 0 || p_n != NULL) {
    aux = p_n;
    p_n = p_n->next;
    pat_node_delete(&aux);
    numero = numero - 1;
  }
  if (!p_n) {
    pattern_delete(&p);
  }
  p->begin = p_n;
}

void pattern_shove(pattern *p, pattern_symbol p_s){
  if(!pattern_is_valid(p)) handle_error(ERROR_PATTERN, "Tentativa de empurrar em um padrão não válido");
  pattern_node *p_node_p_n = pat_node_create(p_s);
  p_node_p_n->next = p->begin;
  p->begin = p_node_p_n;
}
