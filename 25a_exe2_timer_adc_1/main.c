
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/adc.h"

/*
 * 0..1.0V: Desligado
 * 1..2.0V: 150 ms
 * 2..3.3V: 400 ms
 */

volatile int g_timer_0, g_timer_1 = 0;

const int LED_PIN_B = 4;

bool timer_0_callback(repeating_timer_t *rt)
{
    g_timer_0 = 1;
    return true; // keep repeating
}

bool timer_1_callback(repeating_timer_t *rt)
{
    g_timer_1 = 1;
    return true; // keep repeating
}

int main()
{
    stdio_init_all();
    //printf("ADC Example, measuring GPIO27\n");

    adc_init();

    adc_gpio_init(28);
    adc_select_input(2);

    gpio_init(LED_PIN_B);
    gpio_set_dir(LED_PIN_B, GPIO_OUT);

    repeating_timer_t timer_0, timer_1;

    if (!add_repeating_timer_ms(300,
                                timer_0_callback,
                                NULL,
                                &timer_0))
    {
       // printf("Failed to add timer\n");
    }

    if (!add_repeating_timer_ms(500,
                                timer_1_callback,
                                NULL,
                                &timer_1))
    {
       // printf("Failed to add timer\n");
    }

    int piscaLED_300, piscaLED_500 = 0;
    int inverte = 0;

    while (1)
    {
        // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
        const float conversion_factor = 3.3f / (1 << 12);
        uint16_t result = adc_read();
        float voltage = result * conversion_factor;
       // printf("voltage: %f V\n", voltage);

        if (voltage >= 1.0 && voltage <= 2.0)
        {
            piscaLED_500 = 0;
            piscaLED_300 = 1;
        }

        else if (voltage > 2.0)
        {
            piscaLED_300 = 0;
            piscaLED_500 = 1;
        }

        else if (voltage < 1.0)
        {

            piscaLED_300 = 0;
            piscaLED_500 = 0;

            gpio_put(LED_PIN_B, 0);
        }

        if (g_timer_0 && piscaLED_300 == 1)
        {

            if (inverte == 0)
            {
                gpio_put(LED_PIN_B, 1);
                inverte = 1;
            }
            else
            {
                gpio_put(LED_PIN_B, 0);
                inverte = 0;
            }

            g_timer_0 = false;
        }

        if (g_timer_1 && piscaLED_500 == 1)
        {

            if (inverte == 0)
            {
                gpio_put(LED_PIN_B, 1);
                inverte = 1;
            }
            else
            {
                gpio_put(LED_PIN_B, 0);
                inverte = 0;
            }

            g_timer_1 = false;
        }

        //sleep_ms(1);
    }

    return 0;
}
