// Created by Oleksandra Baga on 06.02.18.
// Eptecon Berlin

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "soc/gpio_struct.h"
#include "driver/spi_master.h"
#include "esp_system.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "spi_app.h"
#include "interrupt_app.h"
#include "adc_app.h"
#include "pwm_app.h"

void app_main(){

    /*********************************************************************************
     * GPIOs Matrix Init
     *********************************************************************************/
    // SPI GPIO
    gpio_config_input(&io_int_conf);
    gpio_init_noise_ISR();
    // LED GPIO
    blink_interupt_init();
    // PWM GPIO
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
     * RTOS Tasks
     *********************************************************************************/
    // ISR Task
    xTaskCreate(gpio_noise_interrupt_task, "Interrupt from the Noise Click on the GPIO", 2048, NULL, 10, NULL);

    // Blink LED Task
    xTaskCreate(&blink_interrupt_task, "blink task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);

    // PWM for Buzzer Task
    xTaskCreate(pwm_buzzer_task, "pwm buzzer task", 4096, NULL, 5, NULL);


    printf("Running...\n");

}