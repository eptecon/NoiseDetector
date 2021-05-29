// Created by Oleksandra Baga on 06.02.18.
// Eptecon Berlin

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "soc/gpio_struct.h"
#include "driver/spi_master.h"
#include "esp_system.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "BootWiFi.h"
#include "sdkconfig.h"
#include "spi_app.h"
#include "interrupt_app.h"
#include "adc_app.h"
#include "pwm_app.h"
#include "mqtt_publish.h"
#include "mqtt_subscribe.h"
#include "queue_int.h"
#include "interrupt.h"


BootWiFi *boot;
clock_t start_t;

extern "C" {
void app_main(void);
}

void app_main(void){

    /*********************************************************************************
    * WI-FI
    *********************************************************************************/
    boot = new BootWiFi();
    boot->boot();

    /*********************************************************************************
    * Queue for Interrupts and Time Scheduling
    *********************************************************************************/
    queueInterrupt = q_new();
    printf("Starting of the program, start_t = %ld\n", clock());

    /*********************************************************************************
     * GPIOs Matrix Init
     *********************************************************************************/
    // Config GPIOs for ISR as Input:
    // New buttons with ISR should be added here.
    // Interrupt from Noise Click
    // Button for MQTT Subscription
    gpio_config_input(&io_int_conf);

    // create a Queue for ISR and hook an ISR Handler
    gpio_init_noise_ISR();

    // LED GPIO as Output without ISR
    blink_interupt_init();
    // PWM GPIO without ISR
    pwm_gpio_init();

    /*********************************************************************************
     * ADC Init
     *********************************************************************************/
    adc_init(&characteristics);

    /*********************************************************************************
     * SPI Init
     *********************************************************************************/
    spi_to_noise_click_bus_init(&buscfg);
    spi_to_noise_click_slave_init(&devcfg, &spi);
    // Transmit the V_in to the SPI
    spi_to_noise_click_transmit_cmd(spi);

    /*********************************************************************************
    * PWM Config and Init
    *********************************************************************************/
    pwm_modul_config(&pwm_config);

    /*********************************************************************************
    * JSON Creation and Tracking
    *********************************************************************************/
    mqtt_msg = mqtt_create_json();
    mqtt_msg = mqtt_create_json_voltage();

    /*********************************************************************************
     * RTOS Tasks
     *********************************************************************************/
    // ISR Tasks: Noise Interrupt and Button Detection
    xTaskCreate(gpio_noise_interrupt_task, "Interrupt from the Noise Click on the GPIO", 2048, NULL, 10, NULL);
    xTaskCreate(gpio_button_subscribe_interrupt_task, "Interrupt from button", 2048, NULL, 10, NULL);

    // Button Debouncing Task
 //   xTaskCreate(button_read_debounce_task, "Button Debouncing Task", 8192, NULL, 10, NULL);

    // Blink LED Task
    xTaskCreate(blink_interrupt_task, "blink task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);

    // PWM for Buzzer Task
    xTaskCreate(pwm_buzzer_task, "pwm buzzer task", 4096, NULL, 5, NULL);

    // Updating JSON via MQTT if interrupt occurred
    xTaskCreate(mqtt_interrupt_task, "JSON via MQTT", 8192, NULL, 5, NULL);


    printf("RUNNING!\n");

    char *mqtt_newMsg = (char*) "factory";
    while(1) {
        mqtt_newMsg = mqtt_subscribe((char *) TOPIC1, mqtt_msg);
        printf("WE GOT: %s\n", mqtt_newMsg);
    }

}
