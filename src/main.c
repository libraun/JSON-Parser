#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "json_parse.h"
#include "stack.h"

void print_tabs(unsigned int num_tabs) {
  for (int i = 0; i < num_tabs; ++i) {

    printf("\t");
  }
}
void print_json_object(struct JSON_OBJECT* object,
		       unsigned int level) {
   
  struct JSON_OBJECT *iter_ptr = object;
  printf("{\n");
  while (iter_ptr != NULL) {
    print_tabs(level);
    printf("'%s' : ", iter_ptr->key);
    if (iter_ptr->type == NESTED_OBJECT_TYPE) {
      print_json_object((struct JSON_OBJECT *) iter_ptr->val,
			level+1);
      print_tabs(level);
      printf("},\n");
    } else {
      printf("'%s',\n",(char *) iter_ptr->val);
    }
    iter_ptr = iter_ptr->next;
  }
  if (level == 1) {
    printf("}\n");
  }
}

int main() {
  struct JSON_OBJECT *json = parse_json("../tests/test_json3.json");
  print_json_object(json, 1);
  return 0;
}
