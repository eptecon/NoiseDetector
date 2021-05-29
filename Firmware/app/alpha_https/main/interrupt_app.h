// Created by Oleksandra Baga on 06.02.18.
// Created for Eptecon Berlin

#pragma once
#include "driver/gpio.h"
#include "sdkconfig.h"

/**********************************************************************************
 * GPIO Matrix for Interrupt
**********************************************************************************/
#define GPIO_INPUT_INTERRUPT GPIO_NUM_33
#define GPIO_INPUT_PIN_SEL  (1ULL << GPIO_INPUT_INTERRUPT)
#define ESP_INTR_FLAG_DEFAULT 0
#define GPIO_OUTPUT_BLINK GPIO_NUM_32

#define INTERRUPT_DELAY 1000

/**********************************************************************************
 * Interrupt GPIO
**********************************************************************************/
extern gpio_config_t io_int_conf; // Interrupt from Noise Click
extern int interrupt_detected;

/**********************************************************************************
 * ISR Handler
**********************************************************************************/
void IRAM_ATTR gpio_isr_handler(void* arg);

/**********************************************************************************
 * GPIO Task
**********************************************************************************/
void gpio_noise_interrupt_task(void* arg);

/**********************************************************************************
 * config GPIO as Input
**********************************************************************************/
void gpio_config_input(gpio_config_t *io_conf);

/**********************************************************************************
 * config GPIO as Input for blinking LED
**********************************************************************************/
void blink_interupt_init();

/**********************************************************************************
 * Blink Function
**********************************************************************************/
void blink_interrupt_task(void *);

/**********************************************************************************
 * config GPIO Interrupt Queue and Handler
**********************************************************************************/
void gpio_init_noise_ISR();


