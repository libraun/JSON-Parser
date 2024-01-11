#include <assert.h>
struct JSON_OBJECT {
  char* key;
  void* val;

  char type;
  struct JSON_OBJECT* next;
};

struct JSON_OBJECT* parse_json(char *filename) {
  char buf[MAX_BUFFER_SIZE];
  memset(buf, 0, sizeof(buf));
  
  FILE *file_ptr = fopen(filename, "r");
  if (file_ptr != NULL) {

    fread(buf, sizeof(char), MAX_BUFFER_SIZE, file_ptr);
    fclose(file_ptr);
    long unsigned int len = strlen(buf);

    struct JSON_OBJECT *ret = parse_tokens(buf, 0, len);

    return ret;
  }
  return NULL;
}

void print_tabs(unsigned int num_tabs) {
  for (int i = 0; i < num_tabs; ++i) {

    printf("\t");
  }
}

void print_json_object(struct JSON_OBJECT* iter_ptr,
		       unsigned int level) {
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

void add_to_object(struct JSON_OBJECT *item,
		   struct JSON_OBJECT **head) {
  struct JSON_OBJECT *iter = *head;
  printf("%s, %p \n",item->key, item->val);
  while (iter->next != NULL) {
    iter = iter->next;
  }
  iter->next = item;
  printf("%s \n", iter->next->key);
}

struct JSON_OBJECT* parse_tokens(char buf[],
				 long unsigned int start,
				 long unsigned int end) {
  if (buf[start] != '{') {
    printf("fuck\n");
    return NULL;
  }
  struct JSON_OBJECT* parsed_object =
    (struct JSON_OBJECT *) malloc(sizeof(struct JSON_OBJECT));
  parsed_object->key = "DEFAULT";
  parsed_object->val = "DEFAULT_VAL";

  parsed_object->type = SIMPLE_OBJECT_TYPE;
  parsed_object->next = NULL;
  
  long unsigned int buf_iloc = start + 1;

  char cur_char;

  char *token = NULL;
  void *value = NULL;
  
  char object_type;
  
  while (buf_iloc < end) {

    cur_char = buf[buf_iloc];
    
    switch(cur_char) {
    case '{':
      long unsigned int nested_obj_end =
	find_nested_object_end(buf, buf_iloc+1, end);
      struct JSON_OBJECT *nested_object =
   	parse_tokens(buf,buf_iloc,nested_obj_end);

      object_type = NESTED_OBJECT_TYPE;
      value = (struct JSON_OBJECT *)
	malloc(sizeof(struct JSON_OBJECT));
      buf_iloc = nested_obj_end;
      break;
      
    case '"':
      long unsigned int start_cpy = buf_iloc+1;
      do {
	++buf_iloc;
      } while (buf[buf_iloc] != '"'
	       && buf_iloc < end);

      printf("String @ indices [%ld, %ld]\n",start_cpy,buf_iloc);
      if (token == NULL) {
	token = (char *) malloc(sizeof(char) * (buf_iloc - start_cpy));
	strncpy(token, &buf[start_cpy], (buf_iloc-start_cpy));
      } else if (value == NULL && token != NULL) {
	object_type = SIMPLE_OBJECT_TYPE;
	value = (char *) malloc(sizeof(char) * (buf_iloc - start_cpy));
	strncpy(value, &buf[start_cpy], (buf_iloc-start_cpy));
      }
      ++buf_iloc;
      break;
      
    case ',':
      if (token != NULL && value != NULL) {
	struct JSON_OBJECT* insert_node = (struct JSON_OBJECT *)
	  malloc(sizeof(JSON_OBJECT));
	//	insert_node->next = NULL;c	
	insert_node->key = (char *) malloc(strlen(token));
	strncpy(insert_node->key, token, strlen(token));
	insert_node->type = object_type;
	
	if (object_type == SIMPLE_OBJECT_TYPE) {
	  insert_node->val = (char *) malloc(strlen(value));
	  strncpy(insert_node->val, value, strlen(value));
	} else {
	  
	  insert_node->val = value;
	}
	add_to_object(insert_node, &parsed_object);

	token = NULL;
	value = NULL;
      }      
      ++buf_iloc;
      break;
      
    default:
      ++buf_iloc;
      break;
    }
  }
  return parsed_object;
}
