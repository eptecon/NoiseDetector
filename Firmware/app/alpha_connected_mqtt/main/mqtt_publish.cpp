/* MQTT Publisher
 *
 *
 * Original Copyright (C) 2006-2016, ARM Limited, All Rights Reserved, Apache 2.0 License.
 * Additions Copyright (C) Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD, Apache 2.0 License.
 * Additions Copyright (C) Copyright 2017 pcbreflux, Apache 2.0 License.
 * Additions Copyright (C) Copyright 2018 eptecon, Apache 2.0 License.
 *
 */
#include <string.h>
#include <stdlib.h>

#include "sdkconfig.h"

#include "MQTTClient.h"
#include "BootWiFi.h"
#include "WiFiEventHandler.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "json_util.h"
#include "interrupt_app.h"
#include "mqtt_publish.h"

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
const int CONNECTED_BIT = BIT0;

#define MQTT_SERVER "gluelogics.iotwebtools.com"
#define MQTT_PORT 1883 // TCP/IP port 1883 is reserved with IANA for use with MQTT.
#define TOPIC1 "v1/devices/me/attributes"
#define TOPIC2 "v1/devices/me/telemetry"
#define TOKEN "SZDjEmfGLXuabfAYHFGg"

static const char *TAG = "MQTT";

// JSON Values
char noise_name[] = "noise_level";
char *mqtt_msg;

unsigned char mqtt_sendBuf[MQTT_BUF_SIZE];
unsigned char mqtt_readBuf[MQTT_BUF_SIZE];


/* FreeRTOS event group to signal when we are connected & ready to make a request */
EventGroupHandle_t wifi_event_group = xEventGroupCreate();

BootWiFi *reboot; // for wi-fi reconnection

char* mqtt_create_json(void){

    mqtt_msg = json_create();
    printf("JSON created.\n");
    return mqtt_msg;
}

char* mqtt_create_json_voltage(void){

    mqtt_msg = json_add(mqtt_msg, noise_name, (char*)"0");
    printf("Voltage record added to JSON.\n");
    return mqtt_msg;
}

/*********************************************************************************
 * Part of ISR: After Interrupt occurred the detected voltage (the level of noise)
 * should be added to JSON and sent to ThingsBoard Dashboard.
 * Access Point with an external value from outside for update the existing JSON
*********************************************************************************/
void mqtt_update_json_voltage(int detected_voltage){

    /*Update JSON Object*/
    char noise_value[32];
    sprintf(noise_value, "%d", detected_voltage);
    mqtt_msg = json_update(mqtt_msg, noise_name, noise_value);
    printf("%s\n", mqtt_msg);
    mqtt_publish((char*) TOPIC2, mqtt_msg);
    printf("Voltage record updated/published.\n");
}

/*********************************************************************************
 * Publishing Function: the char[] for JSON should be created and updated in advance, the function
 * will convert the object into MQTT message and send into related topic with related token
 * NOTE: change token to access another dashboard
*********************************************************************************/
void mqtt_publish(char *topic, char *msgbuf){

    int ret;
    Network network;
    MQTTClient client;
    MQTTString clientId;
    NetworkInit(&network);
    MQTTPacket_connectData data;
    MQTTString user;
    MQTTMessage message;

    ESP_LOGD(TAG, "Start MQTT ...");

    ESP_LOGD(TAG,"NetworkConnect %s:%d ...", MQTT_SERVER, MQTT_PORT);
    ret = NetworkConnect(&network, (char*) MQTT_SERVER, MQTT_PORT);
    if (ret != 0) {
        ESP_LOGI(TAG, "NetworkConnect not SUCCESS: %d", ret);
        goto exit;
    }

    ESP_LOGI(TAG,"MQTTClientInit  ...");
    MQTTClientInit(&client, &network,
                   2000,                 // command_timeout_ms
                   mqtt_sendBuf,         //sendbuf,
                   MQTT_BUF_SIZE,        //sendbuf_size,
                   mqtt_readBuf,         //readbuf,
                   MQTT_BUF_SIZE         //readbuf_size
    );

    clientId = MQTTString_initializer;
    clientId.cstring = (char*) "NoiseDetector";

    user = MQTTString_initializer;
    user.cstring = (char*) TOKEN; // Noise

    data = MQTTPacket_connectData_initializer;
    data.clientID          = clientId;
    data.username		   = user;
    data.willFlag          = 0;
    data.MQTTVersion       = 4; // 3 = 3.1 4 = 3.1.1
    data.keepAliveInterval = 60;
    data.cleansession      = 1;

    ESP_LOGI(TAG,"MQTTConnect  ...");
    ret = MQTTConnect(&client, &data);
    if (ret != SUCCESS) {
        ESP_LOGI(TAG, "MQTTConnect not SUCCESS: %d", ret);
        goto exit;
    }

    ESP_LOGI(TAG, "MQTTPublish  ... %s",msgbuf);
    message.qos = QOS0;
    message.retained = false;
    message.dup = false;
    message.payload = (void*)msgbuf;
    message.payloadlen = strlen(msgbuf);

    ret = MQTTPublish(&client, topic, &message);
    if (ret != SUCCESS) {
        ESP_LOGI(TAG, "MQTTPublish not SUCCESS: %d", ret);
        goto exit;
    }

    for(int countdown = 5; countdown >= 0; countdown--) {
        if(countdown%10==0) {
            ESP_LOGI(TAG, "%d...", countdown);
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    exit:
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
    ESP_LOGI(TAG, "MQTTDisconnect ...");
    for(int countdown = 5; countdown >= 0; countdown--) {
        if(countdown%10==0) {
            ESP_LOGI(TAG, "%d...", countdown);
        }
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    printf("Disconnected!\n");
}



 
