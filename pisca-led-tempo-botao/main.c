/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h> 
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

int const LED = 5;
int const BTN = 28;

volatile int btn_flag;
volatile bool rep_flag = false;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge
    btn_flag = 1;
    
  } else if (events == 0x8) { // rise edge
    btn_flag = 2;
  }
} 

bool timer_0_callback(repeating_timer_t *rt) {
    rep_flag = 1;
    return true; // keep repeating
}


int main() {
    stdio_init_all();
    repeating_timer_t timer_0;


    gpio_init(BTN);
    gpio_set_dir(BTN, GPIO_IN);
    gpio_pull_up(BTN);

    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);

    gpio_set_irq_enabled_with_callback(
    BTN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    uint32_t start_ms;
    uint32_t end_ms;
    
    int status_led = 0;

    while (true) {
        if (btn_flag ==1){ //btn apertado
            
            cancel_repeating_timer(&timer_0);
            start_ms = to_ms_since_boot(get_absolute_time());
        }
        else if(btn_flag ==2){
            end_ms = to_ms_since_boot(get_absolute_time());
            uint32_t intervalo = end_ms - start_ms;
            

            add_repeating_timer_ms(intervalo, timer_0_callback, NULL, &timer_0);
        }

        if (rep_flag){
            
            status_led = !status_led;
            rep_flag = 0;
            gpio_put(LED, status_led);
        }

    }

    return 0;
}