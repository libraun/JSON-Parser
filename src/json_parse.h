#ifndef JSON_PARSE_
#define JSON_PARSE_

#include <string.h>
#include <stdio.h>
#include "stack.h"

typedef struct __JSON_OBJECT_ {
  char *key;
  void *val;

  struct JSON_OBJECT *next;
} JSON_OBJECT;

struct JSON_OBJECT* parse_json(const char* filename);

long unsigned int find_nested_object_end(const char buf[],
					 const long unsigned int start,
					 const long unsigned int end);

struct JSON_OBJECT* parse_tokens(const char buf[],
				 const long unsigned int size,
				 const long unsigned int start,
				 const long unsigned int end);
#include "json_parse.c"

#endif
