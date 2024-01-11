#ifndef JSON_PARSE_
#define JSON_PARSE_

#include <string.h>
#include <stdio.h>
#include "stack.h"

#define SIMPLE_OBJECT_TYPE 1
#define NESTED_OBJECT_TYPE 2

#define MAX_BUFFER_SIZE 4096

typedef struct __JSON_OBJECT_ {
  char *key;
  void *val;

  char type;
  struct JSON_OBJECT *next;
} JSON_OBJECT;

struct JSON_OBJECT* parse_json(char* filename);

long unsigned int find_nested_object_end(char buf[],
					 long unsigned int start,
					 long unsigned int end);

struct JSON_OBJECT* parse_tokens(char buf[],
				 long unsigned int start,
				 long unsigned int end);
#include "json_parse.c"

#endif
