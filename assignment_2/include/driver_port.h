#ifndef DRIVER_PORT_H_
#define DRIVER_PORT_H

#include "S32K144.h"
#include "../Core/Include/core_cm4.h"
#include <stdint.h>
/*
 * PORT Driver for S32K144 (CMSIS)
 * Provides basic pin multiplexing and interrupt configuration
 */

/* Enumeration for PORT modules */
typedef enum
{
	DRIVER_PORTA = 0,
	DRIVER_PORTB,
	DRIVER_PORTC,
	DRIVER_PORTD,
	DRIVER_PORTE
} Driver_PortInstance;

/* Pin mux selection (matches PORT_PCR[MUX] field) */
typedef enum
{
	DRIVER_PORT_MUX_DISABLED = 0U, /* Pin disabled (analog) */
	DRIVER_PORT_MUX_GPIO = 1U, /*GPIO*/
	DRIVER_PORT_MUX_ALT2 = 2U,
	DRIVER_PORT_MUX_ALT3 = 3U,
	DRIVER_PORT_MUX_ALT4 = 4U,
	DRIVER_PORT_MUX_ALT5 = 5U,
	DRIVER_PORT_MUX_ALT6 = 6U,
	DRIVER_PORT_MUX_ALT7 = 7U
} Driver_PortMux;

/* PORT interrupt configuration (IRQC field in PCR[n] */
typedef enum
{
	DRIVER_PORT_IRQ_DISABLED =		0U, /* Interrupt DMA disabled */
	DRIVER_PORT_IRQ_DMA_RISING = 	1U,
	DRIVER_PORT_IRQ_DMA_FAILING = 	2U,
	DRIVER_PORT_IRQ_DMA_EITHER = 	3U,
	DRIVER_PORT_IRQ_LOGIC_ZERO =	8U,
	DRIVER_PORT_IRQ_RISING_EDGE =	9U,
	DRIVER_PORT_IRQ_FAILING_EDGE =	10U,
	DRIVER_PORT_IRQ_EITHER_EDGE = 	11U,
	DRIVER_PORT_IRQ_LOGIC_ONE = 	12U
} Driver_PortIrqConfig;

/* Public API */
#ifdef __cpluscplus
extern "C" {
#endif

/* === Basic clock/pin functions === */
/* Enable PCC clock for the selected PORT */
void DRIVER_PORT_EnableClock(Driver_PortInstance port);

/* Configure pin mux for a specific pin */
void DRIVER_PORT_PinMux(Driver_PortInstance port,
						uint8_t pin,
						Driver_PortMux mux);
void DRIVER_PORT_PullConfig(Driver_PortInstance port,
							uint8_t pin,
							uint8_t enable,
							uint8_t pullup);
/* === Interrupt functions === */
void DRIVER_PORT_PinInterruptConfig(Driver_PortInstance port,
									uint8_t pin,
									Driver_PortIrqConfig irqMode);

void DRIVER_PORT_ClearInterruptFlag(Driver_PortInstance port, uint8_t pin);

/* Callback registration (user handler) */
typedef void (*Driver_PortCallback)(uint8_t pin);
void DRIVER_PORT_RegisterCallback(Driver_PortInstance port, Driver_PortCallback cb);

#ifdef __cplusplus
}
#endif

#endif /* DRIVER_PORT_H_ */




















