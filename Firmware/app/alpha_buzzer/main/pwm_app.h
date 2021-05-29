// Created by Oleksandra Baga on 13.02.18.
// Eptecon Berlin

#pragma once
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"
#include "pwm_app.h"
#include "interrupt_app.h"

#define GPIO_PWM0A_OUT 21   //Set GPIO 19 as PWM0A
#define FREQUENCY 2000
#define DUTY_CYCLE 50

extern mcpwm_config_t pwm_config;

/**********************************************************************************
 * PWM GPIO Init
**********************************************************************************/
void pwm_gpio_init();

/**********************************************************************************
 * PWM Config
**********************************************************************************/
void pwm_modul_config(mcpwm_config_t *pwm_config);

/**********************************************************************************
 * GPIO Buzzer Task
**********************************************************************************/
void pwm_buzzer_task();

