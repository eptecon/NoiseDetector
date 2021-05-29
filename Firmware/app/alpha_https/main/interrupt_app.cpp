/* Created by Oleksandra Baga
 * Created for Eptecon Berlin
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "adc_app.h"
#include "spi_app.h"
#include "interrupt_app.h"

gpio_config_t io_int_conf; // Interrupt from Noise Click
static xQueueHandle gpio_noise_interrupt_queue = NULL;
int interrupt_detected = 0;
int count;

/**********************************************************************************
 *  Using FreeRTOS with C++
**********************************************************************************/

/**********************************************************************************
 * ISR Handler
**********************************************************************************/
void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_noise_interrupt_queue, &gpio_num, NULL);
}

/**********************************************************************************
 * GPIO Task
**********************************************************************************/
void gpio_noise_interrupt_task(void* arg) {
    uint32_t io_num;
    while(1) {
        if(xQueueReceive(gpio_noise_interrupt_queue, &io_num, portMAX_DELAY)) {
            printf("%d mV\n", adc1_to_voltage(ADC1_TEST_CHANNEL, &characteristics));
            printf("GPIO[%d]: #%d Interrupt\n", io_num, count);
            interrupt_detected = 1;
            count++;
        }
        else {
            interrupt_detected = 0;
        }
    }
}

/**********************************************************************************
 * config GPIO as Input
**********************************************************************************/
void gpio_config_input(gpio_config_t *io_int_conf){

    //interrupt of rising edge
    io_int_conf->intr_type = GPIO_INTR_POSEDGE;
    //bit mask of the pins
    io_int_conf->pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode
    io_int_conf->mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_int_conf->pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(io_int_conf);
}

/**********************************************************************************
 * config GPIO as Input for blinking LED
**********************************************************************************/
void blink_interupt_init(){

    gpio_pad_select_gpio(GPIO_OUTPUT_BLINK);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(GPIO_OUTPUT_BLINK, GPIO_MODE_OUTPUT);
    // turn off the led before interrupt arises
    gpio_set_level(GPIO_OUTPUT_BLINK, 0);
}

/**********************************************************************************
 * Blink Function
**********************************************************************************/
void blink_interrupt_task(void* arg){
    while(1) {
        if (interrupt_detected) {
            // Turn off the LED
            gpio_set_level(GPIO_OUTPUT_BLINK, 1);
            // Delay
            vTaskDelay(INTERRUPT_DELAY / portTICK_RATE_MS);
            // Turn off
            gpio_set_level(GPIO_OUTPUT_BLINK, 0);
            interrupt_detected = 0;
        }
        else {
            interrupt_detected = 0;
            vTaskDelay(10 / portTICK_RATE_MS);
        }
    }
}

/**********************************************************************************
 * config GPIO Interrupt Queue and Handler
**********************************************************************************/
void gpio_init_noise_ISR(){

    //create a queue to handle gpio event from isr
    gpio_noise_interrupt_queue = xQueueCreate(10, sizeof(uint32_t));
    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_INTERRUPT, gpio_isr_handler, (void*) GPIO_INPUT_INTERRUPT);
    //hook isr handler for specific gpio pin
}