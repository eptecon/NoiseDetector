#ifndef JSON_UTIL
#define JSON_UTIL

#include "cJSON.h"

char* json_create(char *value1, char *value2);
char* json_update(char *obj, char *value1, char *value2);

#endif /* JSON_UTIL  */