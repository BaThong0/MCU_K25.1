/**
 * @file    driver_gpio.c
 * @author  Vo Ba Thong
 * @brief   driver for gpiogpio based on CMSIS standard.
 * @details GPIO's driver based on CMSIS standard
 */
#include "driver_gpio.h"

/* Pin and port declaration table */
const pin_desp_t pin_table[] =
{
    [BUTTON1]   = {DRIVER_PORTC, 12U},
    [BUTTON2]   = {DRIVER_PORTC, 13U},
    [LED_RED]   = {DRIVER_PORTD, 15U},
    [LED_GREEN] = {DRIVER_PORTD, 16U}
};

#define GPIO_MAX_PINS           32U
#define PIN_IS_AVAILABLE(n)     ((n) < GPIO_MAX_PINS)

static inline GPIO_Type* get_gpio_base(Driver_PortInstance port)
{
	switch(port)
	{
		case DRIVER_PORTA:	return IP_PTA;
		case DRIVER_PORTB:	return IP_PTB;
		case DRIVER_PORTC:	return IP_PTC;
		case DRIVER_PORTD:	return IP_PTD;
		case DRIVER_PORTE:	return IP_PTE;
		default: 		   	return IP_PTA;
	}
}

/* ======================== Driver Functions =======================*/
/* Setup GPIO Interface */
static int32_t ARM_GPIO_Setup (ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event)
{
  int32_t result = ARM_DRIVER_OK;
  if (PIN_IS_AVAILABLE(pin_table[pin].pin))
  {
	  /* Enable clock for port */
	  DRIVER_PORT_EnableClock(pin_table[pin].port);
	  /* Configure pin to GPIO */
	  DRIVER_PORT_PinMux(pin_table[pin].port, pin_table[pin].pin, DRIVER_PORT_MUX_GPIO);

	  /* Save callback */
	  if(cb_event)
	  {
		  DRIVER_PORT_RegisterCallback(pin_table[pin].port, cb_event);
	  }
  }
  else
  {
    result = ARM_GPIO_ERROR_PIN;
  }

  return result;
}

/* Set GPIO pin direction */
static int32_t ARM_GPIO_SetDirection (ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction)
{
  int32_t result = ARM_DRIVER_OK;

  if (PIN_IS_AVAILABLE(pin_table[pin].pin))
  {
	GPIO_Type* gpio = get_gpio_base(pin_table[pin].port);

    switch (direction)
    {
      case ARM_GPIO_OUTPUT:
    	  gpio->PDDR |= (1UL << pin_table[pin].pin);
        break;
      case ARM_GPIO_INPUT:
    	  gpio->PDDR &= ~(1UL << pin_table[pin].pin);
        break;
      default:
        result = ARM_DRIVER_ERROR_PARAMETER;
        break;
    }
  }
  else
  {
    result = ARM_GPIO_ERROR_PIN;
  }

  return result;
}

/* Set GPIO output mode */
static int32_t ARM_GPIO_SetOutputMode(ARM_GPIO_Pin_t pin, ARM_GPIO_OUTPUT_MODE mode)
{
	/* Do Nothing */
}

/* Set GPIO pull resistor */
static int32_t ARM_GPIO_SetPullResistor(ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor)
{
	int32_t result = ARM_DRIVER_OK;
	if(PIN_IS_AVAILABLE(pin_table[pin].pin))
	{
		if(resistor == ARM_GPIO_PULL_UP)
		{
			DRIVER_PORT_PullConfig(pin_table[pin].port, pin_table[pin].pin, ENABLE, ARM_GPIO_PULL_UP);
		}
		else
		{
			DRIVER_PORT_PullConfig(pin_table[pin].port, pin_table[pin].pin, ENABLE, ARM_GPIO_PULL_DOWN);
		}
	}
	else
	{
		result = ARM_GPIO_ERROR_PIN;
	}

	return result;
}

/* Set GPIO event trigger */
static int32_t ARM_GPIO_SetEventTrigger (ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger)
{
  int32_t result = ARM_DRIVER_OK;

  if (PIN_IS_AVAILABLE(pin_table[pin].pin))
  {
	 Driver_PortIrqConfig irqMode;

    switch (trigger)
    {
      case ARM_GPIO_TRIGGER_NONE:
    	  irqMode = DRIVER_PORT_IRQ_DISABLED;
        break;
      case ARM_GPIO_TRIGGER_RISING_EDGE:
    	  irqMode = DRIVER_PORT_IRQ_RISING_EDGE;
        break;
      case ARM_GPIO_TRIGGER_FALLING_EDGE:
    	  irqMode = DRIVER_PORT_IRQ_FAILING_EDGE;
        break;
      case ARM_GPIO_TRIGGER_EITHER_EDGE:
    	  irqMode = DRIVER_PORT_IRQ_EITHER_EDGE;
        break;
      default:
        result = ARM_DRIVER_ERROR_PARAMETER;
        break;
    }
    DRIVER_PORT_PinInterruptConfig(pin_table[pin].port, pin_table[pin].pin, irqMode);
  }
  else
  {
    result = ARM_GPIO_ERROR_PIN;
  }

  return result;
}

/* Set GPIO output value */
static void ARM_GPIO_SetOutput (ARM_GPIO_Pin_t pin, uint32_t val)
{

  if (PIN_IS_AVAILABLE(pin_table[pin].pin))
  {
	  GPIO_Type* gpio = get_gpio_base(pin_table[pin].port);

	  if(val)
	  {
		  gpio->PSOR = (1UL << pin_table[pin].pin);
	  }
	  else
	  {
		  gpio->PCOR = (1UL << pin_table[pin].pin);
	  }
  }
}

/* Get GPIO input value */
static uint32_t ARM_GPIO_GetInput (ARM_GPIO_Pin_t pin)
{
  uint32_t val = 0U;

  if (PIN_IS_AVAILABLE(pin_table[pin].pin))
  {
	  GPIO_Type* gpio = get_gpio_base(pin_table[pin].port);

	  return (gpio->PDIR >> pin_table[pin].pin) & 1UL;
  }
  else
  {
	  val = ARM_GPIO_ERROR_PIN;
  }
  return val;
}

/* Callback function */
void Button_Event (ARM_GPIO_Pin_t pin, uint32_t event)
{
    if (event == ARM_GPIO_TRIGGER_FALLING_EDGE)
    {
        /* Detect which button triggered the interrupt */
        if (pin == pin_table[BUTTON1].pin)
        {
            static uint32_t red_state = 0;
            red_state ^= 1;
            ARM_GPIO_SetOutput(LED_RED, red_state);
        }
        else if (pin == pin_table[BUTTON2].pin)
        {
            static uint32_t green_state = 0;
            green_state ^= 1;
            ARM_GPIO_SetOutput(LED_GREEN, green_state);
        }
    }
}

/* GPIO driver access structure */
ARM_DRIVER_GPIO Driver_GPIO0 = {
  ARM_GPIO_Setup,
  ARM_GPIO_SetDirection,
  ARM_GPIO_SetOutputMode,
  ARM_GPIO_SetPullResistor,
  ARM_GPIO_SetEventTrigger,
  ARM_GPIO_SetOutput,
  ARM_GPIO_GetInput
};

/* END OF FILE */
