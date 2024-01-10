#include "stdlib.h"

union node {
  char val;
  union node* next;
};

union stack {
  union node* head;
  int size;
};

void push(union stack* stack, char val) {
  union node* insert_ptr = stack->head;
  while (insert_ptr != NULL) {

    insert_ptr = insert_ptr->next;
  }
  insert_ptr = (union node*) malloc(sizeof(union node));
  insert_ptr->val = val;

  stack->size = stack->size + 1;
}

char peek(union stack* s) {
  
  if (s->size > 0) {

    return s->head->val;
  }
  return 0;
}

/* Returns 0 on success or -1 on fail */
char pop(union stack* s) {
  
  if (s->size > 0) {

    s->head = s->head->next;
    s->size = s->size - 1;

    return 0;
  }
  return -1;
}
