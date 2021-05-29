#pragma once
#define MQTT_BUF_SIZE 1000
#define MQTT_SERVER "gluelogics.iotwebtools.com"
#define MQTT_PORT 1883 // TCP/IP port 1883 is reserved with IANA for use with MQTT.
#define TOPIC1 "v1/devices/me/attributes"
#define TOPIC2 "v1/devices/me/telemetry"
#define TOKEN "3VHw2OQjLKn7prQnPz57"

#include "esp_event.h"
#include "cJSON.h"
#include "json_util.h"

char* mqtt_create_json(void);

char* mqtt_create_json_voltage(void);

char* mqtt_create_json_led();

char* mqtt_create_json_buzzer();

char* mqtt_create_json_battery();

char* mqtt_create_json_threshold(void);

void mqtt_update_json_voltage(int detected_voltage);

void mqtt_update_json_led();

void mqtt_update_json_buzzer();

void mqtt_update_json_battery();

void mqtt_update_json_threshold(int threshold);

void mqtt_publish(char *topic, char *msgbuf);

extern char *mqtt_msg;
