// Created by Oleksandra Baga on 13.02.18.
// Eptecon Berlin

#include <stdio.h>
#include "string.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_attr.h"
#include "soc/rtc.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"
#include "pwm_app.h"

mcpwm_config_t pwm_config;

/**********************************************************************************
 * PWM GPIO Init
**********************************************************************************/
void pwm_gpio_init(){

    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM0A_OUT);
}

/**********************************************************************************
 * PWM Config
**********************************************************************************/
void pwm_modul_config(mcpwm_config_t *pwm_config){

    pwm_config->frequency = FREQUENCY;    //frequency = 1000Hz
    pwm_config->cmpr_a = DUTY_CYCLE;       //duty cycle of PWMxA = 60.0%
    pwm_config->counter_mode = MCPWM_UP_COUNTER;
    pwm_config->duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, pwm_config);   //Configure PWM0A with above settings
    mcpwm_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);


}

/**********************************************************************************
 * GPIO Buzzer Task
**********************************************************************************/
void pwm_buzzer_task(void *arg){
    while(1) {
        if (interrupt_detected) {
            mcpwm_start(MCPWM_UNIT_0, MCPWM_TIMER_0);
            vTaskDelay(INTERRUPT_DELAY / portTICK_RATE_MS);
            mcpwm_stop(MCPWM_UNIT_0, MCPWM_TIMER_0);
            interrupt_detected = 0;
        }

        else {
            interrupt_detected = 0;
            vTaskDelay(10 / portTICK_RATE_MS);
        }
    }
}