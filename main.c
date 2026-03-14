#include "stm32f411xe.h"
#include <stdint.h>

int main(void) {
    // Enable PA0
    RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // Make PA0 alternate function so that it can be controlled by the TIMER
    GPIOA -> MODER &= ~GPIO_MODER_MODE0_Msk;
    GPIOA -> MODER |= GPIO_MODER_MODE0_1;

    // Wire PA0 to AF0 (0001) which maps to TIMER 1/TIMER 2
    GPIOA -> AFR[0] &= ~(15 << 0);
    GPIOA -> AFR[0] |= (1 << 0);

    // Enable TIMER 2
    RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;
    
    // Set the PSC and ARR so that the counter will reset every 20ms
    TIM2 -> PSC = 1599;
    TIM2 -> ARR = 199;

    // Set the CCMR1 as an output by setting CC1S to 00
    TIM2 -> CCMR1 &= ~TIM_CCMR1_CC1S_Msk;

    // Use the CNT >= CCR1 inequality by setting CCMR1 OC1M to 110 
    TIM2 -> CCMR1 &= ~TIM_CCMR1_OC1M_Msk; 
    TIM2 -> CCMR1 |= (6U << TIM_CCMR1_OC1M_Pos); // 6 == 110
    TIM2 -> CCMR1 &= ~TIM_CCMR1_OC2M_Msk; 
    TIM2 -> CCMR1 |= (6U << TIM_CCMR1_OC2M_Pos); // 6 == 110

    // Enable CCER
    TIM2 -> CCER |= TIM_CCER_CC1E;
    TIM2 -> CCER |= TIM_CCER_CC2E;

    // Turn on the Counter
    TIM2 -> CR1 |= TIM_CR1_CEN;


    uint32_t stage = 0;
    while (1) {
        switch (stage) {
        case 0:
            TIM2 -> CCR1 = 15;
            break;
        case 1:
            TIM2 -> CCR1 = 20;
            break;
        case 2:
            TIM2 -> CCR1 = 15;
            break;
        default:
            break;
        }
       
        if (stage > 2) {
            stage = 0;
        }

        for (volatile int i = 0; i < 25000; i++) {
        }
        stage++;
    };
}


/* Stub for -nostdlib compilation to satisfy the Reset_Handler in startup_stm32f411xe.s */
void __libc_init_array(void) { /* Empty stub */ }
