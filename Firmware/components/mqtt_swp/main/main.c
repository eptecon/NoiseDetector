/* Main -  GlueLogics Project
 *
 *
 *
 */
#include <stdlib.h>

#include "sdkconfig.h"

#include "mqtt_publish.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "mqtt_publish.h"

//#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"


void app_main()
{	
    nvs_flash_init();
    //initialise_wifi();
    xTaskCreate(&mqtt_task, "MQTT", 8192, NULL, 5, NULL);
}
