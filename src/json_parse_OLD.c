#include "stack.h"

struct JSON_OBJECT {
  char* key;
  void* val;

  char type;
  struct JSON_OBJECT* next;
};

struct JSON_OBJECT* parse_json(char *filename) {
  FILE *file_ptr = fopen(filename, "r");
  if (file_ptr != NULL) {
    char buf[MAX_BUFFER_SIZE];
    memset(buf, 0, sizeof(buf));
    
    fread(buf, sizeof(char), MAX_BUFFER_SIZE, file_ptr);
    fclose(file_ptr);

    long unsigned int len = strlen(buf);

    struct JSON_OBJECT *ret = parse_tokens(buf, len, 0, len);

    return ret;
  }
  return NULL;
}

long unsigned int find_nested_object_end(char buf[],
					 long unsigned int start,
					 long unsigned int size) {
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

struct JSON_OBJECT* parse_tokens(char buf[],
				 long unsigned int size,
				 long unsigned int start,
				 long unsigned int end) {
  if (buf[start] != '{') {
    return NULL;
  }
  struct JSON_OBJECT* parsed_object =
    (struct JSON_OBJECT *) malloc(sizeof(struct JSON_OBJECT));
  parsed_object->key = "DEFAULT";
  parsed_object->val = "DEFAULT_VAL";
  parsed_object->type = SIMPLE_OBJECT_TYPE;
  
  struct JSON_OBJECT* iter_object =
    (struct JSON_OBJECT *) malloc(sizeof(struct JSON_OBJECT));
  parsed_object->next = iter_object;
  
  long unsigned int buf_iloc = start + 1;
  char cur_char;
  char* token;
  while (buf_iloc < end &&
	 buf[buf_iloc] != 0) {
    
    cur_char = buf[buf_iloc];
   
    switch(cur_char) {
      //    case '}':
      //return parsed_object->next;
    case '{':
      long unsigned int nested_obj_end =
	find_nested_object_end(buf, buf_iloc+1, end);
      
      struct JSON_OBJECT *nested_object =
   	parse_tokens(buf,size,buf_iloc,nested_obj_end);

      iter_object->val = nested_object;
      iter_object->type = NESTED_OBJECT_TYPE;
      
      buf_iloc = nested_obj_end + 1;

      iter_object = iter_object->next;
      break;

    case '"':
      
      long unsigned int start_cpy = buf_iloc+1;
      do {
	++buf_iloc;
      } while ( (buf[buf_iloc] != '"')
	       && (buf_iloc < end)
		&& (buf[buf_iloc] != 0));
		
      token = (char *) malloc(sizeof(char)*(buf_iloc-start_cpy));

      strncpy(token, &buf[start_cpy], buf_iloc - start_cpy);
      if (iter_object->key == NULL) {
	iter_object->key = token;
      } else {
	iter_object->val = token;
	iter_object->type = SIMPLE_OBJECT_TYPE;
      }
      ++buf_iloc;
      break;

    case ',':
      iter_object->next = (struct JSON_OBJECT *)
	malloc(sizeof(struct JSON_OBJECT)); //= parse_tokens(buf,size,buf_iloc+1,end);
      iter_object = iter_object->next;
      iter_object->next = NULL;
     
      ++buf_iloc;
      break;
      
    default:
      ++buf_iloc;
      break;
    }
  }  
  return parsed_object->next;
}
