#include "pattern.h"
#include "trie.h"

int main(){
  pattern *p = pattern_create();
  pattern_shove(p, 1);
  pattern_shove(p, 0);
  pattern_shove(p, 0);
  trie *t = trie_create();
  trie_node *t_n = trie_add_pattern_and_return_last_elem(t,p);
  pattern *p1 = trie_retrieve_pattern(t_n);
  pattern_list(p1);
  pattern_delete(&p1);
  pattern_list(p1);
}
