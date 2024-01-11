#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "json_parse.h"
#include "stack.h"

int main(int argc, char **argv) {
  struct JSON_OBJECT *json = parse_json(argv[1]);
  print_json_object(json, 1);
  return 0;
}
