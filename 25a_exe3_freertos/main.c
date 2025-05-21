/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

const int BTN_PIN_B_1 = 28;
const int BTN_PIN_B_2 = 26;

const int BTN_PIN_Y_1 = 21;
const int BTN_PIN_Y_2 = 18;

const int LED_PIN_B = 8;
const int LED_PIN_Y = 13;


QueueHandle_t xQueueLED1;
QueueHandle_t xQueueLED2;

int status;

void btn_callback(uint gpio, uint32_t events)
{
    int status;

    if (events == 0x4)
    {

        if (gpio == BTN_PIN_Y_1)
        {
            status = 1;
            xQueueSendFromISR(xQueueLED1, &status, 0);
        }

        else if (gpio == BTN_PIN_Y_2)
        {
            status = 0;
            xQueueSendFromISR(xQueueLED1, &status, 0);
        }

        else if (gpio == BTN_PIN_B_1)
        {
            status = 1;
            xQueueSendFromISR(xQueueLED2, &status, 0);
        }

        else if (gpio == BTN_PIN_B_2)
        {
            status = 0;
            xQueueSendFromISR(xQueueLED2, &status, 0);
        }
    }

    else if (events == 0x8)
    {
    }

    // btn_data.id = gpio;
    // btn_data.status = events;
    // xQueueSendFromISR(xQueueBtn, &btn_data, 0);
}

void led_1_task(void *p)
{
    gpio_init(LED_PIN_Y);
    gpio_set_dir(LED_PIN_Y, GPIO_OUT);

    int status=0;

    while (1)
    {

        if (xQueueReceive(xQueueLED1, &status, 1))
        {
            //printf("STATUS AMARELO: %d\n", status);
        }

        if (status == 1)
        {

            gpio_put(LED_PIN_Y, 1);
            vTaskDelay(pdMS_TO_TICKS(25));
            gpio_put(LED_PIN_Y, 0);
            vTaskDelay(pdMS_TO_TICKS(25));
        }
        

        vTaskDelay(pdMS_TO_TICKS(1));
    }

}

void led_2_task(void *p)
{

    gpio_init(LED_PIN_B);
    gpio_set_dir(LED_PIN_B, GPIO_OUT);

    int status;

    while (1)
    {

       if (xQueueReceive(xQueueLED2, &status, 1))
        {
            //printf("STATUS AZUL: %d\n", status);
        }

        if (status == 1)
        {

            gpio_put(LED_PIN_B, 1);
            vTaskDelay(pdMS_TO_TICKS(25));
            gpio_put(LED_PIN_B, 0);
            vTaskDelay(pdMS_TO_TICKS(25));
        }
        

        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

int main()
{
    stdio_init_all();

    xQueueLED1 = xQueueCreate(2, sizeof(status));
    xQueueLED2 = xQueueCreate(2, sizeof(status));
    //printf("Start RTOS \n");

    gpio_init(BTN_PIN_Y_1);
    gpio_set_dir(BTN_PIN_Y_1, GPIO_IN);
    gpio_pull_up(BTN_PIN_Y_1);
    gpio_set_irq_enabled_with_callback(BTN_PIN_Y_1, GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    gpio_init(BTN_PIN_Y_2);
    gpio_set_dir(BTN_PIN_Y_2, GPIO_IN);
    gpio_pull_up(BTN_PIN_Y_2);
    gpio_set_irq_enabled(BTN_PIN_Y_2, GPIO_IRQ_EDGE_FALL, true);

    gpio_init(BTN_PIN_B_1);
    gpio_set_dir(BTN_PIN_B_1, GPIO_IN);
    gpio_pull_up(BTN_PIN_B_1);
    gpio_set_irq_enabled(BTN_PIN_B_1, GPIO_IRQ_EDGE_FALL, true);

    gpio_init(BTN_PIN_B_2);
    gpio_set_dir(BTN_PIN_B_2, GPIO_IN);
    gpio_pull_up(BTN_PIN_B_2);
    gpio_set_irq_enabled(BTN_PIN_B_2, GPIO_IRQ_EDGE_FALL, true);

    xTaskCreate(led_1_task, "LED_1_Task", 256, NULL, 1, NULL);
    xTaskCreate(led_2_task, "LED_2_Task", 256, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
        ;
}