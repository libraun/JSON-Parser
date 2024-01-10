#include "stack.h"

#define BUFFER_SIZE 4096

struct JSON_OBJECT {
  char* key;
  void* val;

  char type;
  struct JSON_OBJECT* next;
};

struct JSON_OBJECT* parse_json(const char *filename) {
  char buf[BUFFER_SIZE];
  memset(buf, 0, sizeof(buf));
  
  FILE *file_ptr = fopen(filename, "r");
  if (file_ptr != NULL) {

    fread(buf, sizeof(char), BUFFER_SIZE, file_ptr);
    fseek(file_ptr, 0, BUFFER_SIZE);
    int sz = ftell(file_ptr);

    struct JSON_OBJECT *ret = parse_tokens(buf, sz, 0, sz);
    fclose(file_ptr);

    return ret;
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
  parsed_object->val = "DEFAULT_VAL";

  parsed_object->type = SIMPLE_OBJECT;
  
  struct JSON_OBJECT* iter_object =
    (struct JSON_OBJECT *) malloc(sizeof(struct JSON_OBJECT));

  parsed_object->next = iter_object;
  
  long unsigned int buf_iloc = start + 1;
  char cur_char;
  while (buf_iloc < end && buf[buf_iloc] != 0) {
    cur_char = buf[buf_iloc];
    
    if (cur_char == '{') {
      long unsigned int nested_obj_end =
	find_nested_object_end(buf, buf_iloc+1, end);
      if (nested_obj_end == size) return NULL;
      struct JSON_OBJECT *nested_object =
   	parse_tokens(buf,size,buf_iloc,nested_obj_end);

      iter_object->val = nested_object;
      iter_object->type = INNER_OBJECT;
      
      buf_iloc = nested_obj_end + 1;
    } else if (cur_char == '"') {
      long unsigned int start_cpy = buf_iloc+1;
      //      iter_object->type = SIMPLE_OBJECT;
      do {
	++buf_iloc;
      } while (buf[buf_iloc] != '"' && buf_iloc < size);
      
      char* token = (char *) malloc(sizeof(char) * (buf_iloc - start_cpy));
      strncpy(token, &buf[start_cpy], buf_iloc - start_cpy);
      
      if (iter_object->key == NULL) {
	iter_object->key = token;
      } else {
	iter_object->val = token;
	iter_object->type = SIMPLE_OBJECT;
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
  }
  return parsed_object;
}
