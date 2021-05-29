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
#include "MQTTClient.h"

extern char *mqtt_msg;
extern int mqtt_connection_status;

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

void mqtt_update_json_threshold(int16_t threshold);

int mqtt_publish(char *topic, char *msgbuf);

void print_publish_status(int status);


