# Blue Pill Challenge 3: PWM Blinky

Object: Blink an LED using PWM
==============================

Hardware Setup:
===============
Install an LED and a series 1k resistor between pin A1 and ground.

Considerations:
===============
The PWM output will use channel 2 of timer 1 (TIM2_CH1) which is output on pin A0.
The LED will be attached to this A0 pin. Since the pin will be driven by a PWM timer,
this is considered an "alternate function" (AF) of the GPIO pin, and must be set as
such in the GPIOA CNF bits.

Using the internal 8 MHz clock, in order to achieve a 1 Hz blink rate with a 50%
cycle, the period needs to be 8E6 with the capture/compare setting of 4E6. However,
since we are using a 16-bit register, with a maximum value of 65,535, the prescaler
must also be used. So, set the period to be 8000 and the capture/compare register to
4000. This will be 1000 times too fast, so we set the prescaler to 1000 (using a
setting of 999 since a prescaler setting of 0 is actually divide-by-1.) This will
result in a period of 8000 x 1000 (8E6 pulses) with a pin state change at 4000 x
1000 (4E6) for a 50% duty cycle.


Software Setup:
===============
1. Enable GPIO Port A in the RCC APB2ENR register.
2. Set up pin configuration/mode for A0 (LED) as Alternate Function (AF) output,
   push-pull, 2 MHz. This means setting the GPIOA CRL register CNF0[1:0] bits as
   0b10, and MODE0[1:0] bits as 0b10. Remember that the default setting for
   CNF0[1:0] is 0b01, so that CNF0[0] bit must be specifically cleared!
3. Enable TIM 2 by setting the RCC_APB1ENR_TIM2EN bit in the RCC APB1ENR register.
4. Set PWM period via the TIM2 ARR register.
5. Set PWM duty cycle via the TIM2 CCR1 register.
6. Set PWM mode 1 by setting the TIM_CCMR1_OC1Mx bits as 110 via the TIM2 CCMR1
   register.
7. Enable output of TIM2 Channel 1 to pin A0 by setting the TIM_CCER_CC1E bit in
   the TIM2 CCER register.
8. Set the prescaler via the TIM2 PSC register to slow down the output frequency.
9. Enable (turn on) the clock counter via the TIM_CR1_CEN bit in the TIM2 CR1
   register.


This project contains the code required to blink an LED using PWM.

<br>
Hardware Requirements and Wiring<br>
Install an LED and series 1k resistor between pin A0 and ground.<br>
<br>
Setup<br>
The default makefile is set up to work with an ST-LINK V2 programming dongle under Linux. For other
setups, open Makefile and edit as required. There are hints in the Makefile for different programming
options.
<br>
Running make will compile and attempt to upload the program to the Blue Pill. Of course the resulting .elf
file can be uploaded manually later.<br>
<br>
Software Requirements:<br>
Editor of your choice<br>
arm-none-eabi-gcc toolchain<br>
make<br>
git-bash (for windows)<br>
STM32_Programmer_CLI.exe Program to upload .elf file to the microcontroller / Blue Pill<br>
