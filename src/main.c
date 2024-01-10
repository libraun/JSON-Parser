#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "json_parse.h"
#include "stack.h"

void print_json_object(struct JSON_OBJECT* object,
		       unsigned int level) {
   
  struct JSON_OBJECT *iter_ptr = object;
  while (iter_ptr != NULL) {

    for (int i = 0; i < level; ++i) {
      printf("\t");
    }
    printf("'%s' : ", iter_ptr->key);
    //    printf("%d\n",iter_ptr->type);
    if (iter_ptr->type = 1) {
      print_json_object((struct JSON_OBJECT *) iter_ptr->val,level+1);
    } else {
      printf("'%s',\n",(char *) iter_ptr->val);
    }    
    iter_ptr = iter_ptr->next;
  }
}

int main() {
  struct JSON_OBJECT* json = parse_json("../tests/test_json3.json");
  unsigned int level = 0;
  printf("yo");
  print_json_object(json, level);
  return 0;
}
