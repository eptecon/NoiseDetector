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

void app_main(){

    /*********************************************************************************
     * GPIO Init
     *********************************************************************************/
    gpio_config_input(&io_int_conf);
    gpio_init_noise_ISR();

    blink_interupt_init();

    /*********************************************************************************
     * ADC Init
     *********************************************************************************/
    adc_init(&characteristics);

    /*********************************************************************************
     * SPI Init
     *********************************************************************************/
    spi_to_noise_click_bus_init(&buscfg);
    spi_to_noise_click_slave_init(&devcfg, &spi);
    // Transmit the Vref to the SPI
    spi_to_noise_click_transmit_cmd(spi);

    /*********************************************************************************
     * RTOS Tasks
     *********************************************************************************/
    // ISR Task
    xTaskCreate(gpio_noise_interrupt_task, "Interrupt from the Noise Click on the GPIO", 2048, NULL, 10, NULL);

    // Blink LED Task
    xTaskCreate(&blink_interrupt_task, "blink_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);

    printf("Running...\n");

}