#pragma once
#define MQTT_BUF_SIZE 1000

#include "esp_event.h"



char* mqtt_create_json(void);

char* mqtt_create_json_voltage(void);

void mqtt_update_json_voltage(int detected_voltage);

void mqtt_publish(char *topic, char *msgbuf);

extern unsigned char mqtt_sendBuf[]; //[MQTT_BUF_SIZE];
extern unsigned char mqtt_readBuf[]; //[MQTT_BUF_SIZE];

extern char *mqtt_msg;
