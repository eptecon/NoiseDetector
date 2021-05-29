#pragma once
#define MQTT_BUF_SIZE 1000
#define MQTT_SERVER "gluelogics.iotwebtools.com"
#define MQTT_PORT 1883 // TCP/IP port 1883 is reserved with IANA for use with MQTT.
#define TOPIC1 "v1/devices/me/attributes"
#define TOPIC2 "v1/devices/me/telemetry"
#define TOKEN "3VHw2OQjLKn7prQnPz57"

#include "esp_event.h"

char* mqtt_create_json(void);

char* mqtt_create_json_voltage(void);

void mqtt_update_json_voltage(int detected_voltage);

void mqtt_publish(char *topic, char *msgbuf);

extern char *mqtt_msg;
