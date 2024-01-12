/** @file "main.c"
 *  @author "Jet Braun" */

#include <stdlib.h>
#include <stdio.h>
#include "json_parse.h"

int main(int argc, char **argv) {
  
  struct JSON_OBJECT *json = parse_json_from_file(argv[1]);

  print_json_object(json, 1);
  return 0;
}
