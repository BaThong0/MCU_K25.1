/**
 * @file    main.c
 * @author  Vo Ba ThongThong
 * @brief   Application entry point.
 * @details Write GPIO, PORT driver for S32K144 using CMSIS, write test application for drivers by toggle LED via buttons
 */

#include "driver_gpio.h"
/* === Pin definitions === */


extern ARM_DRIVER_GPIO Driver_GPIO0;
/* Button interrupt callback */
static void Button_Event (ARM_GPIO_Pin_t pin, uint32_t event)
{
    if (event & ARM_GPIO_TRIGGER_FALLING_EDGE)
    {
        /* Detect which button triggered the interrupt */
        if (pin == BUTTON1)
        {
            static uint32_t red_state = 0;
            red_state ^= 1;   // toggle
            Driver_GPIO0.SetOutput(LED_RED, red_state);
        }
        else if (pin == BUTTON2)
        {
            static uint32_t green_state = 0;
            green_state ^= 1; // toggle
            Driver_GPIO0.SetOutput(LED_GREEN, green_state);
        }
    }
}
int main(void) {
	/* LED Setup */
    Driver_GPIO0.Setup(LED_RED, NULL);
    Driver_GPIO0.SetDirection(LED_RED, ARM_GPIO_OUTPUT);
    Driver_GPIO0.SetOutput(LED_RED, 0);

    Driver_GPIO0.Setup(LED_GREEN, NULL);
    Driver_GPIO0.SetDirection(LED_GREEN, ARM_GPIO_OUTPUT);
    Driver_GPIO0.SetOutput(LED_GREEN, 0);

    /* Button Setup */
    Driver_GPIO0.Setup(BUTTON1, Button_Event);
    Driver_GPIO0.SetDirection(BUTTON1, ARM_GPIO_INPUT);
    Driver_GPIO0.SetEventTrigger(BUTTON1, ARM_GPIO_TRIGGER_FALLING_EDGE);


    Driver_GPIO0.Setup(BUTTON2, Button_Event);
    Driver_GPIO0.SetDirection(BUTTON2, ARM_GPIO_INPUT);
    Driver_GPIO0.SetEventTrigger(BUTTON2, ARM_GPIO_TRIGGER_FALLING_EDGE);
    while(1) {
    	//Nothing happens here, the interrupts handle everything
    }
    return 0;
}

