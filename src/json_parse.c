#include "stack.h"

#define BUFFER_SIZE 4096

struct JSON_OBJECT {
  char* key;
  void* val;

  struct JSON_OBJECT* next;
};

struct JSON_OBJECT* parse_json(const char *filename) {
  char buf[BUFFER_SIZE];
  memset(buf, 0, sizeof(buf));
  
  FILE *file = fopen(filename, "r");
  if (file != NULL) {

    fread(buf, sizeof(char), BUFFER_SIZE, file);
    fseek(file, 0, BUFFER_SIZE);
    int sz = ftell(file);

    return parse_tokens(buf, sz, 0, sz);
  }
  return NULL;
}

long unsigned int find_nested_object_end(const char buf[],
					 const long unsigned int start,
					 const long unsigned int size) {
  long unsigned int idx = start;
  unsigned int opening_bracket_count = 1;
  unsigned int closing_bracket_count = 0;

  while (opening_bracket_count != closing_bracket_count
	 && idx < size) {
    switch (buf[idx]) {
    case '{':
      opening_bracket_count = opening_bracket_count + 1;
      break;
    case '}':
      closing_bracket_count = closing_bracket_count + 1;
      break;
    default: break;
    }
    ++idx;
  }
  return idx;
}

struct JSON_OBJECT* parse_tokens(const char buf[],
				 const long unsigned int size,
				 const long unsigned int start,
				 const long unsigned int end) {
  if (buf[start] != '{') {

    return NULL;
  }
  struct JSON_OBJECT* parsed_object =
    (struct JSON_OBJECT *) malloc(sizeof(struct JSON_OBJECT));
  parsed_object->key = "DEFAULT";
  struct JSON_OBJECT* iter_object = parsed_object;
  
  unsigned int buf_iloc = start + 1;

  char cur_char;

  char is_key = 0; // Boolean Value

  while (buf_iloc < end && buf[buf_iloc] != 0) {

    cur_char = buf[buf_iloc];    

    if (cur_char == '{' && is_key == 0) {
      long unsigned int nested_obj_end =
	find_nested_object_end(buf, buf_iloc+1, end);
      void* nested_object =
	parse_tokens(buf,size,buf_iloc,nested_obj_end);

      iter_object->val = nested_object;
      
      buf_iloc = nested_obj_end + 1;
    } else if (cur_char == '"') {
      unsigned int start_cpy = buf_iloc+1;

      while (buf[buf_iloc+1] != '"') {
	++buf_iloc;
      }
      printf("%d, %d\n", start_cpy,buf_iloc);
      char* token = (char *)
	malloc(sizeof(char) * (buf_iloc - start_cpy));
      strncpy(token, &buf[start_cpy], buf_iloc - start_cpy);
      if (is_key) {
	iter_object->key = token;
      } else {
	iter_object->val = token;
      }
      ++buf_iloc;
    } else if (buf[buf_iloc] == ',') {

      iter_object->next = (struct JSON_OBJECT*)
	malloc(sizeof(struct JSON_OBJECT));
      iter_object = iter_object->next;
      
      ++buf_iloc;
    } else {
      ++buf_iloc;
    }
    is_key = !is_key;
  }
  return parsed_object;
}
