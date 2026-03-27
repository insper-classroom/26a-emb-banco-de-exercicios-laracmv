#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/adc.h"

const int LED_PIN_Y = 10;
const int LED_PIN_B = 14;

const int BTN_PIN_Y = 26;
const int BTN_PIN_B = 19;

volatile int yellow_flag = 0;
volatile int blue_flag = 0;
volatile int g_timer_0 = 0;
volatile bool timer_fired = false;

void btn_callback(uint gpio, uint32_t events) {
    if (gpio ==  BTN_PIN_Y){
        yellow_flag = 1;
    }
    else if (gpio == BTN_PIN_B){
        blue_flag = 1;
    }
}

bool timer_0_callback(repeating_timer_t *rt){
    g_timer_0 = 1;
    return true;
}

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    timer_fired = true;

    // Can return a value here in us to fire in the future
    return 0;
}

int main()
{
    stdio_init_all();
    repeating_timer_t timer_0;

    gpio_init(LED_PIN_Y);
    gpio_set_dir(LED_PIN_Y, GPIO_OUT);

    gpio_init(LED_PIN_B);
    gpio_set_dir(LED_PIN_B, GPIO_OUT);

    gpio_init(BTN_PIN_Y);
    gpio_set_dir(BTN_PIN_Y, GPIO_IN);
    gpio_pull_up(BTN_PIN_Y);

    gpio_init(BTN_PIN_B);
    gpio_set_dir(BTN_PIN_B, GPIO_IN);
    gpio_pull_up(BTN_PIN_B);

    gpio_set_irq_enabled_with_callback(BTN_PIN_Y, GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    gpio_set_irq_enabled(BTN_PIN_B, GPIO_IRQ_EDGE_FALL, true);

    int i = 0;
    alarm_id_t alarm;
    int yellow_prim = 0;
    int blue_prim = 0;
    int yellow_sec=  0;
    int blue_sec = 0;

    while (1) {
    
        
        if (yellow_flag){
            add_repeating_timer_ms(0.2 * 1000, timer_0_callback,NULL,&timer_0);
            alarm = add_alarm_in_ms(1000, alarm_callback, NULL, false);
            yellow_flag = 0;


            if (!yellow_sec){
                yellow_prim = 1;

            }
        
        }
        

        else if (blue_flag){
            add_repeating_timer_ms(0.5 * 1000, timer_0_callback,NULL,&timer_0);
            alarm = add_alarm_in_ms(2000, alarm_callback, NULL, false);
            blue_flag= 0;
        
            if (!blue_sec){
                blue_prim = 1;
            }

        
        }

        if (g_timer_0 && (yellow_prim || yellow_sec) && !timer_fired){
            i = !i;
            gpio_put(LED_PIN_Y, i);
            g_timer_0 = 0;
        
        }
        else if (g_timer_0 && (blue_prim || blue_sec) && !timer_fired){
            i = !i;
            gpio_put(LED_PIN_B, i);
            g_timer_0 = 0;
        
        }

        if (timer_fired){

            cancel_repeating_timer(&timer_0);
            cancel_alarm(alarm);
            i = 0;
            gpio_put(LED_PIN_Y, i);
            gpio_put(LED_PIN_B, i);
            timer_fired = false;

            if (blue_prim) {
                blue_prim = 0;
                yellow_sec = 1;
                yellow_flag = 1;
            }
            else if(yellow_prim){
                yellow_prim = 0;
                blue_sec = 1;
                blue_flag = 1;
            }

            else if(yellow_sec){
                yellow_sec = 0;

            }
            else if(blue_sec){
                blue_sec = 0;
            }

    
            
        }

        
        
    }

    return 0;
}