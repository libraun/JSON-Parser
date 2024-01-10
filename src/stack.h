#ifndef STACK_
#define STACK_

#include <stdio.h>

typedef union __node_ {
  char val;
  union node *next;
} node;

typedef union __stack_ {
  union node *head;
  int size;
} stack;
#include "stack.c"
#endif
