/**
 * @file    main.c
 * @author  Vo Ba Thong
 * @brief   Application entry point.
 * @details Write GPIO, PORT driver for S32K144 using CMSIS, write test application for drivers by toggle LED via buttons
 */

#include "driver_gpio.h"
extern ARM_DRIVER_GPIO Driver_GPIO0;

int main(void) {
	/* LED Setup */
    Driver_GPIO0.Setup(LED_RED, NULL);
    Driver_GPIO0.SetDirection(LED_RED, ARM_GPIO_OUTPUT);
    Driver_GPIO0.SetOutput(LED_RED, 0);
    Driver_GPIO0.SetOutput(LED_RED, 1);

    Driver_GPIO0.Setup(LED_GREEN, NULL);
    Driver_GPIO0.SetDirection(LED_GREEN, ARM_GPIO_OUTPUT);
    Driver_GPIO0.SetOutput(LED_GREEN, 0);
    Driver_GPIO0.SetOutput(LED_GREEN, 1);

    /* Button Setup */
    Driver_GPIO0.Setup(BUTTON1, Button_Event);
    Driver_GPIO0.SetDirection(BUTTON1, ARM_GPIO_INPUT);
    Driver_GPIO0.SetPullResistor(BUTTON1, ARM_GPIO_PULL_UP);
    Driver_GPIO0.SetEventTrigger(BUTTON1, ARM_GPIO_TRIGGER_FALLING_EDGE);


    Driver_GPIO0.Setup(BUTTON2, Button_Event);
    Driver_GPIO0.SetDirection(BUTTON2, ARM_GPIO_INPUT);
    Driver_GPIO0.SetPullResistor(BUTTON2, ARM_GPIO_PULL_UP);
    Driver_GPIO0.SetEventTrigger(BUTTON2, ARM_GPIO_TRIGGER_FALLING_EDGE);

    while(1)
    {
    	/* Do nothing */
    }
    return 0;
}

/* END OF FILE */
