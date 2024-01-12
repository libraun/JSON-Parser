/** @file "json_parse.c"
 *  @author "Jet Braun" */

#ifndef JSON_PARSE_
#define JSON_PARSE_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_BUFFER_SIZE 8192
#define MAX_STRING_LEN 64

#define NULL_VALUE 0
#define STRING_VALUE 1
#define NESTED_VALUE 2

typedef struct __JSON_OBJECT_ {
  char *key;
  void *val;

  char type;
  struct JSON_OBJECT *next;
} JSON_OBJECT;

/** @function "parse_json_from_file"
 *  @desc "Given a filename, opens a file stream and fully parses a JSON_OBJECT."

 *  @param "filename" The path to the target file to parse into a JSON_OBJECT.
 *  @return The parsed JSON_OBJECT */
struct JSON_OBJECT *parse_json_from_file(const char* filename);

/** @function "find_nested_object_end"
 *  @desc "Finds the index of the next outer bracket from 'start'"
 *  
 *  @param "buf" The buffer in which we expect to find an outer bracket.
 *  @param "start" The index in the buffer to begin searching.
 *  @param "end" The index in the buffer to end searching regardless
 *               of whether or not a closing bracket was found.
 *  @return An integer index in the buffer that holds the next outer bracket that is
 *          on level with the starting bracket position.  */
long unsigned int find_nested_object_end(const char buf[],
					 const long unsigned int start,
					 const long unsigned int end);

/** @function "find_nested_object_end"
 *  @desc "Finds the index of the next outer bracket from 'start'"
 *
 *  @param "buf" The buffer from which JSON objects will be parsed.
 *  @param "start" The index in the buffer to begin parsing mapped values.
 *  @param "end" The index in the buffer to end parsing, regardless of whether 
 *               a JSON object has been successfully parsed.
 *  @return A parsed JSON list object. */

struct JSON_OBJECT *parse_tokens(const char buf[],
				 const long unsigned int start,
				 const long unsigned int end);

/** @function "print_json_object"
 *  @desc "Prints a given JSON object from top to bottom, formatted
 *         in the JSON style."
 *
 *  @param "object" A pointer to the object to print.
 *  @param "level" The current 'level' (or 'depth') in the given JSON object.
 *                 Used for printing a formatted number of tabs. */
void print_json_object(struct JSON_OBJECT *object,
		       unsigned int level);
#include "json_parse.c"
#endif
