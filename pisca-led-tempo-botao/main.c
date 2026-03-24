#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

int const LED = 5;
int const BTN = 28;

volatile int btn_flag = 0;
volatile int rep_flag = 0;

volatile uint64_t start_us = 0;
volatile uint64_t end_us = 0;

void btn_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_FALL) { // apertou
        start_us = to_us_since_boot(get_absolute_time());
        btn_flag = 1;
    } 
    else if (events & GPIO_IRQ_EDGE_RISE) { // soltou
        end_us = to_us_since_boot(get_absolute_time());
        btn_flag = 2;
    }
}

bool timer_0_callback(repeating_timer_t *rt) {
    rep_flag = 1;
    return true;
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
        BTN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback
    );

    int status_led = 0;
    bool timer_ativo = false;

    while (true) {

        if (btn_flag == 1) { // pressionado
            if (timer_ativo) {
                cancel_repeating_timer(&timer_0);
                timer_ativo = false;
            }

            status_led = 0;
            gpio_put(LED, status_led);

            btn_flag = 0;
            rep_flag = 0;
        }

        else if (btn_flag == 2) { // soltou
            uint64_t intervalo = end_us - start_us;

            if (intervalo < 2000) intervalo = 2000; // mínimo seguro

            if (timer_ativo) {
                cancel_repeating_timer(&timer_0);
            }

            add_repeating_timer_us(intervalo / 2, timer_0_callback, NULL, &timer_0);
            timer_ativo = true;

            btn_flag = 0;
        }

        if (rep_flag) {
            status_led = !status_led;
            gpio_put(LED, status_led);
            rep_flag = 0;
        }
    }

    return 0;
}