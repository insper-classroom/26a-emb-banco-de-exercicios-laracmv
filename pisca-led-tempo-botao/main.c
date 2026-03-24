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
#include <inttypes.h> // Essencial para PRIu32

int const LED = 5;
int const BTN = 28;

volatile int btn_flag;
volatile int rep_flag = 0;

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

    uint64_t start_us;
    uint64_t end_us;
    
    int status_led = 0;

    while (true) {
        if (btn_flag ==1){ //btn apertado
            
            cancel_repeating_timer(&timer_0);
            status_led = 0;
            gpio_put(LED, status_led);

            start_us = to_us_since_boot(get_absolute_time());

            btn_flag = 0;
            rep_flag  = 0;
        }
        else if(btn_flag ==2){
            end_us = to_us_since_boot(get_absolute_time());
            uint64_t intervalo = end_us - start_us;
            
            
            add_repeating_timer_us(intervalo / 2, timer_0_callback, NULL, &timer_0);

            btn_flag = 0;
        }

        if (rep_flag){
          
            status_led = !status_led;
            gpio_put(LED, status_led);

            rep_flag = 0;
        }

    }

    return 0;
}