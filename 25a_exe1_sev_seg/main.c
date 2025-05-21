#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"

const int SEVEN_SEG_A = 2;
const int SEVEN_SEG_B = 3;
const int SEVEN_SEG_C = 4;
const int SEVEN_SEG_D = 5;
const int SEVEN_SEG_E = 6;
const int SEVEN_SEG_F = 7;
const int SEVEN_SEG_G = 8;


const int BTN_PIN_G = 28;

volatile int btnStatus = 0;


void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge
    //printf("f \n");
    btnStatus = 1;
  } else if (events == 0x8) { // rise edge
    //printf("r \n");
  }
}

void seven_seg_init(){
    gpio_init(SEVEN_SEG_A);
    gpio_set_dir(SEVEN_SEG_A, GPIO_OUT);

    gpio_init(SEVEN_SEG_B);
    gpio_set_dir(SEVEN_SEG_B, GPIO_OUT);

    gpio_init(SEVEN_SEG_C);
    gpio_set_dir(SEVEN_SEG_C, GPIO_OUT);

    gpio_init(SEVEN_SEG_D);
    gpio_set_dir(SEVEN_SEG_D, GPIO_OUT);

    gpio_init(SEVEN_SEG_E);
    gpio_set_dir(SEVEN_SEG_E, GPIO_OUT);

    gpio_init(SEVEN_SEG_F);
    gpio_set_dir(SEVEN_SEG_F, GPIO_OUT);

    gpio_init(SEVEN_SEG_G);
    gpio_set_dir(SEVEN_SEG_G, GPIO_OUT);


    gpio_put(SEVEN_SEG_A, 1);
    gpio_put(SEVEN_SEG_B, 1);
    gpio_put(SEVEN_SEG_C, 1);
    gpio_put(SEVEN_SEG_D, 1);
    gpio_put(SEVEN_SEG_E, 1);
    gpio_put(SEVEN_SEG_F, 1);

  
}

void seven_seg_display(int val){
    gpio_put(SEVEN_SEG_A, 0);
    gpio_put(SEVEN_SEG_B, 0);
    gpio_put(SEVEN_SEG_C, 0);
    gpio_put(SEVEN_SEG_D, 0);
    gpio_put(SEVEN_SEG_E, 0);
    gpio_put(SEVEN_SEG_F, 0);
    gpio_put(SEVEN_SEG_G, 0);

if(val == 0){
    gpio_put(SEVEN_SEG_A, 1);
    gpio_put(SEVEN_SEG_B, 1);
    gpio_put(SEVEN_SEG_C, 1);
    gpio_put(SEVEN_SEG_D, 1);
    gpio_put(SEVEN_SEG_E, 1);
    gpio_put(SEVEN_SEG_F, 1);
}


else if(val == 1){
    gpio_put(SEVEN_SEG_B, 1);
    gpio_put(SEVEN_SEG_C, 1);
}

else if(val == 2){
    gpio_put(SEVEN_SEG_A, 1);
    gpio_put(SEVEN_SEG_B, 1);
    gpio_put(SEVEN_SEG_D, 1);
    gpio_put(SEVEN_SEG_E, 1);
    gpio_put(SEVEN_SEG_G, 1);
}

else if(val == 3){
    gpio_put(SEVEN_SEG_A, 1);
    gpio_put(SEVEN_SEG_B, 1);
    gpio_put(SEVEN_SEG_C, 1);
    gpio_put(SEVEN_SEG_D, 1);
    gpio_put(SEVEN_SEG_G, 1);
}

else if(val == 4){
    gpio_put(SEVEN_SEG_B, 1);
    gpio_put(SEVEN_SEG_C, 1);
    gpio_put(SEVEN_SEG_F, 1);
    gpio_put(SEVEN_SEG_G, 1);
}

else if(val == 5){
    gpio_put(SEVEN_SEG_A, 1);
    gpio_put(SEVEN_SEG_C, 1);
    gpio_put(SEVEN_SEG_D, 1);
    gpio_put(SEVEN_SEG_F, 1);
    gpio_put(SEVEN_SEG_G, 1);
}

else if(val == 6){
    gpio_put(SEVEN_SEG_A, 1);
    gpio_put(SEVEN_SEG_C, 1);
    gpio_put(SEVEN_SEG_D, 1);
    gpio_put(SEVEN_SEG_E, 1);
    gpio_put(SEVEN_SEG_F, 1);
    gpio_put(SEVEN_SEG_G, 1);
}

else if(val == 7){
    gpio_put(SEVEN_SEG_A, 1);
    gpio_put(SEVEN_SEG_B, 1);
    gpio_put(SEVEN_SEG_C, 1);
}

else if(val == 8){
    gpio_put(SEVEN_SEG_A, 1);
    gpio_put(SEVEN_SEG_B, 1);
    gpio_put(SEVEN_SEG_C, 1);
    gpio_put(SEVEN_SEG_D, 1);
    gpio_put(SEVEN_SEG_E, 1);
    gpio_put(SEVEN_SEG_F, 1);
    gpio_put(SEVEN_SEG_G, 1);
}

else if(val == 9){
    gpio_put(SEVEN_SEG_A, 1);
    gpio_put(SEVEN_SEG_B, 1);
    gpio_put(SEVEN_SEG_C, 1);
    gpio_put(SEVEN_SEG_D, 1);
    gpio_put(SEVEN_SEG_F, 1);
    gpio_put(SEVEN_SEG_G, 1);
}

}

int main() {
    stdio_init_all();

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);
  
    gpio_set_irq_enabled_with_callback(
        BTN_PIN_G, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    int valorHex = 0;

    seven_seg_init();
    
    while (true) {

        //seven_seg_display(valorHex);
        //sleep_ms(1000);
        //valorHex++;

        if(btnStatus == 1){
           // printf("btnStatus 1");
            valorHex++;
            if(valorHex == 10)  valorHex = 0;
            seven_seg_display(valorHex);

            btnStatus = 0;
        }

    }
}
