#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "json_parse.h"
#include "stack.h"

int main() {
  struct JSON_OBJECT* json = parse_json("test_json.json");
  printf("%s\n",json->key);
  return 0;
}
