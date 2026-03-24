#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"

const int BTN = 22;
const int SWITCH = 28;

const int LEDS[5] = {2,3,4,5,6};
const int LED_1 = 2;
const int LED_2 = 3;
const int LED_3 = 4;
const int LED_4 = 5;
const int LED_5 = 6;

volatile int btn_flag;
volatile int switch_flag;

void btn_callback(uint gpio, uint32_t events){
    if (events == 0x4){
        if (gpio == BTN){ //fall edge
            btn_flag = 1;
        }
        if(gpio == SWITCH){
            switch_flag = 0;
        }   

    }

    else if (events == 0x8){ // rise edge
        
        if(gpio == SWITCH){
            switch_flag = 1;
        }  

    }
    
    
}


void bar_init(){ //inicializa pinos
    gpio_init(BTN);
    gpio_set_dir(BTN, GPIO_IN);
    gpio_pull_up(BTN);

    gpio_init(LED_1);
    gpio_set_dir(LED_1, GPIO_OUT);

    gpio_init(LED_2);
    gpio_set_dir(LED_2, GPIO_OUT);

    gpio_init(LED_3);
    gpio_set_dir(LED_3, GPIO_OUT);

    gpio_init(LED_4);
    gpio_set_dir(LED_4, GPIO_OUT);

    gpio_init(LED_5);
    gpio_set_dir(LED_5, GPIO_OUT);

    gpio_init(SWITCH);
    gpio_set_dir(SWITCH, GPIO_IN);
    gpio_pull_up(SWITCH);

}

int estado_leds = 0;
int i_leds = 0;

void bar_display(int val){ //preenche leds da barra

    gpio_put(LEDS[val], estado_leds);

}



int main() {
    stdio_init_all();

    bar_init();

    gpio_set_irq_enabled_with_callback(BTN, GPIO_IRQ_EDGE_FALL| GPIO_IRQ_EDGE_RISE, true, &btn_callback);

    gpio_set_irq_enabled(SWITCH, GPIO_IRQ_EDGE_FALL| GPIO_IRQ_EDGE_RISE, true);
    
    while (true) {
    
        if (btn_flag && switch_flag){ //decrementa
            printf("BOTÃO APERTADO, SWITCH EM %d \n", switch_flag);

            estado_leds = 0;
            printf("O estado é %d\n", estado_leds);

            printf("O incrementador dos leds é: %d\n", i_leds);
            bar_display(i_leds);

            if (i_leds > 0 ){
                i_leds--;
            }   
            else {
                i_leds = 0;
            }
            

            

            btn_flag = 0;
        }
        else if (btn_flag && !switch_flag){ //incrementa
            printf("BOTÃO APERTADO, SWITCH EM %d \n", switch_flag);

            estado_leds = 1;
            printf("O estado é %d\n", estado_leds);

            printf("O incrementador dos leds é: %d\n", i_leds);
            bar_display(i_leds);

            if (i_leds < 4){
                i_leds++;
            }
            else{
                i_leds = 4;
            }
            
            

            btn_flag = 0;
        }

    }
}

