// Created by Oleksandra Baga on 05.02.18.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "driver/spi_master.h"
#include "esp_system.h"
#include "esp_adc_cal.h"

#include "vref_loop.h"
#include "spi_app.h"

spi_device_handle_t spi;
spi_bus_config_t buscfg;
spi_device_interface_config_t devcfg;

void app_main(void) {
    /*********************************************************************************
     * SPI Init
     *********************************************************************************/
    spi_to_noise_click_bus_init(&buscfg);
    spi_to_noise_click_slave_init(&devcfg, &spi);

    // Transmit the Vref to the SPI
    //spi_to_noise_click_transmit_cmd(spi);

    printf("Running...\n");

    while(1) {

        int v_start = 0;
        int step = 500;

        printf("Setting Vref:\n");
        for (int vref = v_start; vref < 4096; vref += step) {
            // send 10 times the same voltage (test for oscilloscope)
            for (int j = 0; j < 5; j++) {
                printf("To transmit decimal: %d\n", vref);
                set_vref_loop(spi, 255, 1);
                vTaskDelay(2000 / portTICK_RATE_MS);
            }
        }
    }
}