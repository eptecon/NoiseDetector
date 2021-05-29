/* JSON Utility
 *
 *
 * Copyright (C) Copyright 2018 eptecon, Apache 2.0 License.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "json_util.h"

/* defines */


/* constants */


/* variables */
cJSON *root;
char *out;


char* json_create(char *value1, char *value2)
{
	//char *out;	
	//cJSON *root;

	root  = cJSON_CreateObject();
	
	cJSON_AddItemToObject(root, "temperature", cJSON_CreateString(value1));
	cJSON_AddItemToObject(root, "humidity", cJSON_CreateString(value2));
	cJSON_AddItemToObject(root, "active", cJSON_CreateString("true"));

	out = cJSON_Print(root);
	//printf("%s\n",out);
	
	return out;
}/*JSON_create*/

char* json_update(char *obj, char *value1, char *value2)
{
	//char *out;	
	//cJSON *obj;
	
	root  = cJSON_Parse(obj);
	//obj  = cJSON_CreateObject();
	
	cJSON_ReplaceItemInObject(root, "temperature", cJSON_CreateString(value1));
	cJSON_ReplaceItemInObject(root, "humidity", cJSON_CreateString(value2));
	cJSON_ReplaceItemInObject(root, "active", cJSON_CreateString("true"));
	//cJSON_AddItemToArray(key, root);

	out = cJSON_Print(root);
	//printf("%s\n",out);
	
	return out;
}/*JSON_update*/