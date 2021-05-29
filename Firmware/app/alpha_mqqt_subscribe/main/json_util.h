#ifndef JSON_UTIL
#define JSON_UTIL

#include "cJSON.h"


#ifdef __cplusplus
extern "C" {
#endif

char* json_create(void);
char* json_add(char *json, char *name, char *value);
char* json_update(char *json, char *name, char *value);

#ifdef __cplusplus
}
#endif

#endif /* JSON_UTIL  */