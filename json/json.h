/*
Please send Bug-Reports to my given mail address.
Feel free to use this project for everything you want. 

Author: Felix Froehling <felix.froehling1@gmail.com>
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <inttypes.h>

#include "helper.h"

struct json_object;
struct json_array;

typedef struct keyvalue {
    char* key;
    char* stringvalue;
    int* intvalue;
    int is_null;
    struct json_object *json_obj;
    struct json_array *json_arr;
} KEYVALUE;

//Datatype for a JSON-Object
typedef struct json_object {
   KEYVALUE** entries;
   int len_entries;
} JSON_OBJECT;

//Datatype for a JSON-Array
typedef struct json_array {
    JSON_OBJECT** entries;
    int len_entries;
} JSON_ARRAY;


//Put your string here to parse it. 0 is returned when string is invalid json, 1 is returned if its valid. 
//In this case you can check for result_obj and  result_arr being unequal to NULL, to check what type of json is there (if you don't know yet)
int parse(char* string, JSON_OBJECT** result_obj, JSON_ARRAY** result_arr);

//Output-Functions print values to screen. Be aware, this is just very basic. Feel free to write your customized output, e.g. with indents.
void outputJsonArray(JSON_ARRAY* arr);
void outputJsonObject(JSON_OBJECT* obj);

//Call this function to release resources when you are done
void freeJsonArray(JSON_ARRAY* arr);
void freeJsonObject(JSON_OBJECT* obj);
