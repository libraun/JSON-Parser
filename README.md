# c_json_parse
A JSON parser built in C.

## Functionality

### parse_json_from_filename(char *filename)

Accepts a path to a file (relative or absolute) and returns a fully parsed JSON data structure.

### add_to_object(struct JSON_OBJECT *item, struct JSON_OBJECT **head)

This function accepts an (allocated) JSON_OBJECT whose fields have been pre-assigned by the user
and appends it to the end of a given JSON_OBJECT. 

Note that in order to append an item to a nested JSON_OBJECT value, the user should supply the 
(void *) value pointed to by a JSON_OBJECT whose type is NESTED_VALUE. See below for more information.

### Accessing the JSON Object

In order to access a JSON_OBJECT returned by "parse_json_from_filename", it is recommended
to refer to the member field "type" for any given JSON_OBJECT node, which indicates
how much memory has been allocated to a given JSON_OBJECT node's property "value".

The user can use these values as they would an enum. Example pseudocode
can be found below for a function that traverses a JSON_OBJECT, printing a node
_only if_ it is of the type "STRING_VALUE":

_void_ traverse_json(struct JSON_OBJECT *my_json_object_ptr) {
  if (my_json_object_ptr->type == NESTED_VALUE) {
    traverse_json(my_json_object_ptr->val); 
  } else if (my_json_object_ptr->type == STRING_VALUE) {
    print_key_value_pair(my_json_object_ptr->key, my_json_object_ptr->val);
  } 
}


