
/* MQTT Subscriber
 *
 *
 * Original Copyright (C) 2006-2016, ARM Limited, All Rights Reserved, Apache 2.0 License.
 * Additions Copyright (C) Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD, Apache 2.0 License.
 * Additions Copyright (C) Copyright 2017 pcbreflux, Apache 2.0 License.
 * Additions Copyright (C) Copyright 2018 eptecon, Apache 2.0 License.
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
#include "sdkconfig.h"
#include "MQTTClient.h"
#include "mqtt_publish.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_system.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"
#include "json_util.h"

unsigned char mqtt_sub_readBuf[MQTT_BUF_SIZE];
unsigned char mqtt_sub_sendBuf[MQTT_BUF_SIZE];
MessageData *received_message;
char *mqtt_recMsg;

static const char *TAG = "MQTT";


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
static void mqtt_message_handler(MessageData *md) {
	ESP_LOGI(TAG, "Topic received!: %.*s %.*s", md->topicName->lenstring.len, md->topicName->lenstring.data, md->message->payloadlen, (char*)md->message->payload);
    mqtt_recMsg = (char *)md->message->payload;
}
#pragma GCC diagnostic pop

char* mqtt_subscribe(char *topic, char *msgbuf){

	int ret;
    Network network;
    MQTTClient client;
    MQTTString clientId;
    NetworkInit(&network);
    MQTTPacket_connectData data;
    MQTTString user;

    ESP_LOGD(TAG, "Start MQTT  Subscribe...");

    ESP_LOGD(TAG,"NetworkConnect %s:%d ...", (char *) MQTT_SERVER, MQTT_PORT);
    ret = NetworkConnect(&network, (char *) MQTT_SERVER, MQTT_PORT);
    if (ret != 0) {
        ESP_LOGI(TAG, "NetworkConnect not SUCCESS: %d", ret);
        goto exit;
    }

    ESP_LOGD(TAG,"MQTTClientInit  ...");
    MQTTClientInit(&client, &network,
                   2000,            // command_timeout_ms
                   mqtt_sub_sendBuf,         //sendbuf,
                   MQTT_BUF_SIZE, //sendbuf_size,
                   mqtt_sub_readBuf,         //readbuf,
                   MQTT_BUF_SIZE  //readbuf_size
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

    ESP_LOGI(TAG, "MQTTSubscribe  ...");
    ret = MQTTSubscribe(&client, "v1/devices/me/rpc/request/+", QOS0, mqtt_message_handler);
    if (ret != SUCCESS) {
        ESP_LOGI(TAG, "MQTTSubscribe: %d", ret);
        goto exit;
    }

    ESP_LOGI(TAG, "MQTTYield  ...");

    ret = MQTTYield(&client, (data.keepAliveInterval+1)*2);
    if (ret != SUCCESS) {
        ESP_LOGI(TAG, "MQTTYield: %d", ret);
        goto exit;
    }

    exit:
    MQTTDisconnect(&client);
    NetworkDisconnect(&network);
    ESP_LOGI(TAG, "MQTTDisconnect ...");
    printf("Disconnected!\n");

    return(mqtt_recMsg);
}
