/*
 * ext_intr.c
 *
 *  Created on: Dec 18, 2025
 *      Author: kiran_z6dopa8
 */


#include"my_led.h"
void extint_init(void)
{
    // 1. Enable AHB1 clock for GPIOA
    //    Without enabling the clock, GPIOA registers cannot be accessed
    RCC->AHB1ENR |= BV(0);

    // 2. Configure PA0 as input mode
    //    MODER0[1:0] = 00 → Input mode
    GPIOA->MODER &= ~(BV(0) | BV(1));

    // 3. Disable internal pull-up / pull-down on PA0
    //    External pull-down is already present on the board (USER button)
    GPIOA->PUPDR &= ~(BV(0) | BV(1));

    // 4. Map PA0 to EXTI line 0 using SYSCFG
    //    EXTICR[0] controls EXTI0–EXTI3
    //    Clearing bits selects Port A for EXTI0
    SYSCFG->EXTICR[0] &= ~(BV(0) | BV(1) | BV(2) | BV(3));

    // 5. Configure EXTI line 0
    //    Unmask EXTI0 interrupt
    EXTI->IMR |= BV(0);

    //    Enable rising-edge trigger on EXTI0
    //    PA0 goes LOW → HIGH when USER button is pressed
    EXTI->RTSR |= BV(0);

    // 6. Enable EXTI0 interrupt in NVIC
    //    Allows the Cortex-M3 core to accept EXTI0 interrupts
    NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void)
{
    // 1. Clear the pending interrupt flag for EXTI line 0
    //    This acknowledges the interrupt to the EXTI controller.
    //    If not cleared, the ISR will be called repeatedly.
    EXTI->PR |= BV(0);

    // 2. Perform the required action for the interrupt
    //    Toggle LED connected to pin 14 (e.g., PD14 on STM32F407)
    //    Keep ISR short and non-blocking.
         led_toggle(14);

}
