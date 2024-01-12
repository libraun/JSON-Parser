/** @file "parse_json.c"
 *  @author "Jet Braun" */

struct JSON_OBJECT {
  char* key;
  void* val;

  char type;
  struct JSON_OBJECT* next;
};

struct JSON_OBJECT *parse_json_from_file(const char *filename) {
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
  printf("{\n");
  while (iter_ptr != NULL) {
    print_tabs(level);
    printf("'%s' : ", iter_ptr->key);
    if (iter_ptr->type == NESTED_VALUE) {
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

void add_to_object(struct JSON_OBJECT *item,
		   struct JSON_OBJECT **head) {
  if (head == NULL) {
    head = &item;
    return;
  }
  struct JSON_OBJECT *iter = *head;
  while (iter->next != NULL) {
    iter = iter->next;
  }
  iter->next = item;
}

struct JSON_OBJECT *parse_tokens(const char buf[],
				 const long unsigned int start,
				 const long unsigned int end) {
  struct JSON_OBJECT *parsed_object =
    (struct JSON_OBJECT *) malloc(sizeof(struct JSON_OBJECT));
  parsed_object->key = "DEFAULT";
  parsed_object->val = "DEFAULT_VAL";

  parsed_object->type = STRING_VALUE;
  parsed_object->next = NULL;
				    
  long unsigned int buf_iloc = start + 1;

  long unsigned int string_start_idx;
  long unsigned int num_chars;
  
  char *token = NULL;
  void *value = NULL;  
  char value_type = NULL_VALUE;

  char cur_char;
  while (buf_iloc < end) {
    cur_char = buf[buf_iloc];
    switch(cur_char) {
    case '{':
      long unsigned int nested_value_end =
	find_nested_object_end(buf, buf_iloc+1, end);
      
      struct JSON_OBJECT *nested_value =
   	parse_tokens(buf,buf_iloc,nested_value_end);

      value_type = NESTED_VALUE;
      //      value = (struct JSON_OBJECT *)
      //.	malloc(sizeof(struct JSON_OBJECT));
      value = nested_value;
      buf_iloc = nested_value_end;
      break;
      
    case '"':
      /* This is basically asking for a buffer overflow on empty
	 strings. This should be addressed. */
      string_start_idx = buf_iloc + 1;
      do {
	++buf_iloc;
      } while (buf_iloc < end &&
	       buf[buf_iloc] != '"');
      
      num_chars = buf_iloc - string_start_idx;
      if (token == NULL) {
	token = (char *) malloc(sizeof(char) * num_chars);
	strncpy(token, &buf[string_start_idx], num_chars);
      } else if (value == NULL) {
	value_type = STRING_VALUE;
	value = (char *) malloc(sizeof(char) * num_chars);
	strncpy(value, &buf[string_start_idx], num_chars);
      }
      ++buf_iloc;
      break;
   
    case ',':
      /** Current char is a comma, meaning we should have two (non-null) fields.
	  Allocate memory for a new JSON node, assign respective values,
	  and add to the end of the JSON_OBJECT. **/
      if (token != NULL && value != NULL) {
	struct JSON_OBJECT* insert_node = (struct JSON_OBJECT *)
	  malloc(sizeof(struct JSON_OBJECT));
	insert_node->next = NULL;	
	insert_node->key = (char *) malloc( strnlen(token, MAX_STRING_LEN) );
	strncpy(insert_node->key, token, strnlen(token, MAX_STRING_LEN));
	insert_node->type = value_type;

	/** Allocate memory to the value of new JSON node
	  * based on which "object_type" it is, and assign accordingly **/
	if (value_type == STRING_VALUE) {
	  insert_node->val = (char *) malloc(strlen(value));
	  strncpy(insert_node->val, value, strlen(value));
	} else {
	  insert_node->val = (struct JSON_OBJECT *)
	    malloc(sizeof(struct JSON_OBJECT));
	  insert_node->val = value;
	}
        add_to_object(insert_node, &parsed_object);

	/* Reset the required JSON_OBJECT fields */
	token = NULL;
	value = NULL;
	value_type = NULL_VALUE;
      }      
      ++buf_iloc;
      break;
      
    default:
      ++buf_iloc;
      break;
    }
  }
  return parsed_object->next;
}
