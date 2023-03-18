//  Challenge 3, PWM Blinky
//  Mike Shegedin, 03/2023

//  Object: Blink an LED using PWM
//  ==============================

//  Hardware Setup:
//  ===============
//  Install an LED and a series 1k resistor between pin A1 and ground.

//  Considerations:
//  ===============
//  The PWM output will use channel 2 of timer 1 (TIM2_CH1) which is output on pin A0.
//  The LED will be attached to this A0 pin. Since the pin will be driven by a PWM timer,
//  this is considered an "alternate function" (AF) of the GPIO pin, and must be set as
//  such in the GPIOA CNF/MODE bits:
//
//      Alternate Function, Push-Pull Output, 2 MHz max speed setting:
//      CNF0[1:0] = 0b10, MODE0[1:0] = 0b10
//
//  Using the internal 8 MHz clock, in order to achieve a 1 Hz blink rate with a 50% duty
//  cycle, the period needs to be 8E6 with the capture/compare setting of 4E6. However,
//  since we are using a 16-bit register, the maximum settable period is 65,535 ticks at
//  a frequency of 8 MHz.
//
//       Period = 65,535 ticks / 8 MHz = 8.19 ms
//    Frequency = 1 / 8.19 ms = 122 Hz
//
//  An LED flashing at 122 Hz is too fast to see. So we need to use a prescaler to slow
//  down the PWM clock. Using a prescaler of 8000:
//
//      8 MHz main clock / 8000 prescaler = 1 kHz PWM clock
//
//  With this, the PWM clock period can easily be set in miliseconds in the TIM2 ARR register and
//  likewise, the duty cycle period in the TIM2 CCR1 register. Nice! For a 1 Hz clock with a 50%
//  duty cycle:
//
//      TIM2->ARR = 1000  (1000 ms PWM period)
//      TIM2->CCR1 = 500  (duty cycle = 500 / 1000 = 50%)


//  Software Setup:
//  ===============
//  1. Enable GPIO Port A in the RCC APB2ENR register.
//  2. Set up pin configuration/mode for A0 (LED) as Alternate Function (AF) output,
//     push-pull, 2 MHz. This means setting the GPIOA CRL register CNF0[1:0] bits as
//     0b10, and MODE0[1:0] bits as 0b10. Remember that the default setting for
//     CNF0[1:0] is 0b01, so that CNF0[0] bit must be specifically cleared!
//  3. Enable TIM 2 by setting the RCC_APB1ENR_TIM2EN bit in the RCC APB1ENR register.
//  4. Set the prescaler via the TIM2 PSC register to slow down the output frequency.
//  5. Set PWM period via the TIM2 ARR register.
//  6. Set PWM duty cycle via the TIM2 CCR1 register.
//  7. Set PWM mode 1 by setting the TIM_CCMR1_OC1Mx bits as 110 via the TIM2 CCMR1
//     register.
//  8. Enable output of TIM2 Channel 1 to pin A0 by setting the TIM_CCER_CC1E bit in
//     the TIM2 CCER register.
//  9. Enable (turn on) the clock counter via the TIM_CR1_CEN bit in the TIM2 CR1
//     register.

#include "stm32f103xb.h"

int
main( void )
{
    //  1. Enable GPIO Port A in the RCC APB2ENR register.
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    //  2. Set up pin configuration/mode for A0 (LED) as Alternate Function (AF) output,
    //     push-pull, 2 MHz. This means setting the GPIOA CRL register CNF0[1:0] bits as
    //     0b10, and MODE0[1:0] bits as 0b10. Remember that the default setting for
    //     CNF0[1:0] is 0b01, so that CNF0[0] bit must be specifically cleared!
    GPIOA->CRL &= ~GPIO_CRL_CNF0_Msk;                   // Clear CNF1[1:0] bits
    GPIOA->CRL |= GPIO_CRL_CNF0_1 | GPIO_CRL_MODE0_1;   // Set CNF1 and MODE1 1-bits

    //  3. Enable TIM 2 by setting the RCC_APB1ENR_TIM2EN bit in the RCC APB1ENR register.
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    //  4. Set the prescaler to divide the PWM clock by 8000 via the TIM2 PSC register.
    TIM2->PSC = 8000;

    //  5. Set PWM period via the TIM2 ARR register.
    TIM2->ARR = 1000;

    //  6. Set PWM duty cycle via the TIM2 CCR1 register.
    TIM2->CCR1 = 500;

    //  7. Set PWM mode 1 by setting the TIM_CCMR1_OC1M_x bits as 110 via the TIM2 CCMR1
    //     register.
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;

    //  8. Enable output of TIM2 Channel 1 to pin A0 by setting the TIM_CCER_CC1E bit in
    //     the TIM2 CCER register.
    TIM2->CCER |= TIM_CCER_CC1E;

    //  9. Enable (turn on) the clock counter via the TIM_CR1_CEN bit in the TIM2 CR1
    //     register.
    TIM2->CR1 |= TIM_CR1_CEN;

    while( 1 )        // End in an endless loop while the PWM does its thing!
    {
    }
} // End main.c
