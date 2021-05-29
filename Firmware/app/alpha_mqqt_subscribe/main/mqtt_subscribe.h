#pragma once

#include "mqtt_publish.h"
#include "MQTTClient.h"

char* mqtt_subscribe(char *topic, char *msgbuf);
