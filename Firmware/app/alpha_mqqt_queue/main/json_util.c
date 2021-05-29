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

cJSON *root;
char *out;

char* json_create(void){

    root = cJSON_CreateObject();
    out = cJSON_Print(root);
    return out;
}

char* json_add(char *json, char *name, char *value)
{
    root = cJSON_Parse(json);
    cJSON_AddItemToObject(root, name, cJSON_CreateString(value));
    out = cJSON_Print(root);
    return out;
}

char* json_update(char *json, char *name, char *value)
{
	root = cJSON_Parse(json);

	cJSON_ReplaceItemInObject(root, name, cJSON_CreateString(value));

    out = cJSON_Print(root);
    return out;
}