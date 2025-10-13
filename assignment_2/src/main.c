/**
 * @file    main.c
 * @author  Vo Ba Thong
 * @brief   Application entry point.
 * @details Write USART driver for S32K144 using CMSIS, write test application for drivers by sending command
 */

#include "driver_gpio.h"
#include "driver_usart.h"
#include "clocks_and_modes.h"
#include <stdio.h>

extern ARM_DRIVER_GPIO Driver_GPIO0;
extern ARM_DRIVER_USART Driver_USART0;
void UART_Callback()
{
	/*Parse data to get the complete command*/
	ProcessCommand;
}

void ProcessCommand(string cmd)
{
	switch(cmd)
	{
		case LED_STATUS:
		    /* Get LED information */
		    Driver_USART0.Send(led_status);
		    break;
		case RED_ON:
		    /* Turn on red led */
		    Driver_GPIO0.SetOutput(LED_RED, 1);
		    break;
		case RED_OFF:
		    /* Turn off red led */
			Driver_GPIO0.SetOutput(LED_RED, 0);
			break;
		case GREEN_ON:
		    /* Turn on green led */
			Driver_GPIO0.SetOutput(LED_GREEN, 1);
			break;
	    case GREEN_OFF:
		    /* Turn off green led */
			Driver_GPIO0.SetOutput(LED_GREEN, 0);
			break;
		case BLUE_ON:
		    /* Turn on blue led */
			Driver_GPIO0.SetOutput(LED_BLUE, 1);
			break;
		case BLUE_OFF:
		    /* Turn off blue led */
			Driver_GPIO0.SetOutput(LED_BLUE, 0);
			break;
		default:
		    printf("\nCommand not available\n");
			break;
	}

}

int main(void) {
	/* Program's data */
	char command[100];
	/* LED Setup */
    Driver_GPIO0.Setup(LED_BLUE, NULL);
    Driver_GPIO0.SetDirection(LED_BLUE, ARM_GPIO_OUTPUT);
    Driver_GPIO0.SetOutput(LED_BLUE, 0);
    Driver_GPIO0.SetOutput(LED_BLUE, 1);

    Driver_GPIO0.Setup(LED_RED, NULL);
    Driver_GPIO0.SetDirection(LED_RED, ARM_GPIO_OUTPUT);
    Driver_GPIO0.SetOutput(LED_RED, 0);
    Driver_GPIO0.SetOutput(LED_RED, 1);

    Driver_GPIO0.Setup(LED_GREEN, NULL);
    Driver_GPIO0.SetDirection(LED_GREEN, ARM_GPIO_OUTPUT);
    Driver_GPIO0.SetOutput(LED_GREEN, 0);
    Driver_GPIO0.SetOutput(LED_GREEN, 1);

	/* USART Setup */
    Driver_USART0.Initialize(UART_Callback);

	SOSC_init_8MHz(); /* Initialize system oscillator for 8 MHz xtal */
    SPLL_init_160MHz(); /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
    NormalRUNmode_80MHz(); /* Init clocks: 80 MHz SPLL & core, 40 MHz bus, 20 MHz flash */
	while(1)
	{
		printf("Enter user's command: ");
		fgets(command, sizeof(command), stdin);
		Driver_USART0.Send(command);
	}
    return 0;
}
