#include "driver_usart.h"
#include "S32K144.h"
#include <stdint.h>
#include "../Core/Include/core_cm4.h"

#define ARM_USART_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0)  /* driver version */

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = { 
    ARM_USART_API_VERSION,
    ARM_USART_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_USART_CAPABILITIES DriverCapabilities = {
    1, /* supports UART (Asynchronous) mode */
    0, /* supports Synchronous Master mode */
    0, /* supports Synchronous Slave mode */
    0, /* supports UART Single-wire mode */
    0, /* supports UART IrDA mode */
    0, /* supports UART Smart Card mode */
    0, /* Smart Card Clock generator available */
    0, /* RTS Flow Control available */
    0, /* CTS Flow Control available */
    0, /* Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE */
    0, /* Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT */
    0, /* RTS Line: 0=not available, 1=available */
    0, /* CTS Line: 0=not available, 1=available */
    0, /* DTR Line: 0=not available, 1=available */
    0, /* DSR Line: 0=not available, 1=available */
    0, /* DCD Line: 0=not available, 1=available */
    0, /* RI Line: 0=not available, 1=available */
    0, /* Signal CTS change event: \ref ARM_USART_EVENT_CTS */
    0, /* Signal DSR change event: \ref ARM_USART_EVENT_DSR */
    0, /* Signal DCD change event: \ref ARM_USART_EVENT_DCD */
    0, /* Signal RI change event: \ref ARM_USART_EVENT_RI */
    0  /* Reserved (must be zero) */
};

//
//   Functions
//

/**
 * @brief Get USART driver's version
 * 
 * @return ARM_DRIVER_VERSION 
 */
static ARM_DRIVER_VERSION ARM_USART_GetVersion(void)
{
  return DriverVersion;
}

/**
 * @brief Get USART driver's capability
 * 
 * @return ARM_USART_CAPABILITIES 
 */
static ARM_USART_CAPABILITIES ARM_USART_GetCapabilities(void)
{
  return DriverCapabilities;
}

/**
 * @brief Initialize for the USART driver
 * 
 * @param cb_event 
 * @return int32_t 
 */
static int32_t ARM_USART_Initialize(ARM_USART_SignalEvent_t cb_event)
{
	/* Config port and enable clock for UART */
	DRIVER_PORT_EnableClock(uart_instance.pcc_index);
	/* Choose clock source */
	IP_PCC->PCCn[uart_instance.pcc_index] |= PCC_PCCn_PCS;
	/* Config pin mux */
	DRIVER_PORT_PinMux(uart_instance.port, uart_instance.pin, DRIVER_PORT_MUX_ALT2);
	/* Set baud rate and other settings */
	ARM_USART_Control(ARM_USART_MODE_ASYNCHRONOUS, 9600);
	NVIC_ClearPendingIRQ(uart_instance.get_IRQ);
	NVIC_EnableIRQ(uart_instance.get_IRQ)
}

/**
 * @brief Uninitialize for the USART driver
 * 
 * @return int32_t 
 */
static int32_t ARM_USART_Uninitialize(void)
{
	/* Reverse the Initialization */
}

/**
 * @brief Control the power of the USART driver
 * 
 * @param state 
 * @return int32_t 
 */
static int32_t ARM_USART_PowerControl(ARM_POWER_STATE state)
{
    switch (state)
    {
    case ARM_POWER_OFF:
        break;

    case ARM_POWER_LOW:
        break;

    case ARM_POWER_FULL:
        break;
    }
    return ARM_DRIVER_OK;
}

/**
 * @brief Send the data through UART peripheral
 * 
 * @param data 
 * @param num 
 * @return int32_t 
 */
static int32_t ARM_USART_Send(const void *data, uint32_t num)
{
	if ((data == NULL) || (num == 0U)) {
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    uart_instance.tx_buf = (const uint8_t *)data;

    for (uint32_t i = 0; i < num; i++) {
        /* Wait until Transmit Data Register Empty (TDRE) flag is set */
        while (!(uart_instance.get_ip->STAT & LPUART_STAT_TDRE_MASK)) {
            /* Wait for transmitter to be ready */
        }

        /* Write the next byte to DATA register */
        uart_instance.get_ip->DATA = uart_instance.tx_buf[i];
    }

    /* Wait for the Transmission Complete (TC) flag to ensure last bit sent */
    while (!(uart_instance.get_ip->STAT & LPUART_STAT_TC_MASK));

    return ARM_DRIVER_OK;
}

/**
 * @brief Receive the data through peripheral
 * 
 * @param data 
 * @param num 
 * @return int32_t 
 */
static int32_t ARM_USART_Receive(void *data, uint32_t num)
{
	uart_instance.rx_buf = (uint8_t *)data;

    for (uint32_t i = 0; i < num; i++)
    {
        /* Wait until Receive Data Register Full (RDRF = 1) */
        while ((uart_instance.get_ip->STAT & LPUART_STAT_RDRF_MASK) == 0U)
        {
            /* wait for data */
        }

        /* Read the received byte */
        uart_insance.rx_buf[i] = (uint8_t)(uart_instance.get_ip->DATA & 0xFFU);
    }

    return ARM_DRIVER_OK;
}

/**
 * @brief Transfer the data through peripheral
 * 
 * @param data_out 
 * @param data_in 
 * @param num 
 * @return int32_t 
 */
static int32_t ARM_USART_Transfer(const void *data_out, void *data_in, uint32_t num)
{
	/* Do nothing */
}

/**
 * @brief Get transmit's data size 
 * 
 * @return uint32_t 
 */
static uint32_t ARM_USART_GetTxCount(void)
{
	/* Do nothing */
}

/**
 * @brief Get receive's data size
 * 
 * @return uint32_t 
 */
static uint32_t ARM_USART_GetRxCount(void)
{
	/* Do nothing */
}

/**
 * @brief Set baud rate and others params
 * 
 * @param control 
 * @param arg 
 * @return int32_t 
 */
static int32_t ARM_USART_Control(uint32_t control, uint32_t arg)
{
	/* For now there in this version, there is only a mode config
	This asynchronous mode is setting as 9600 baud, 1 stop bit, 8-bit format, no parity
	*/
	uint32_t mode = control & ARM_USART_MODE_ASYNCHRONOUS;
	switch(mode)
	{
		case ARM_USART_MODE_ASYNCHRONOUS:
		uart_instance.get_IP->BAUD = 0x0F000034;
		uart_instance.get_IP->CTRL = 0x000C0000;
	}
	/*Set baud rate*/
}

/**
 * @brief Get USART's status 
 * 
 * @return ARM_USART_STATUS 
 */
static ARM_USART_STATUS ARM_USART_GetStatus(void)
{
	/* Do nothing */
}

/**
 * @brief Control the modem-related hardware flow control signals of UART peripheral
 * 
 * @param control 
 * @return int32_t 
 */
static int32_t ARM_USART_SetModemControl(ARM_USART_MODEM_CONTROL control)
{
	/* Do nothing */
}

/**
 * @brief Get the modem's status of UART peripheral
 * 
 * @return ARM_USART_MODEM_STATUS 
 */
static ARM_USART_MODEM_STATUS ARM_USART_GetModemStatus(void)
{
}

/**
 * @brief Callback function of UART 
 * 
 * @param event 
 */
static void ARM_USART_SignalEvent(uint32_t event)
{
    // function body
}

// End USART Interface

ARM_DRIVER_USART Driver_USART0 = {
    ARM_USART_GetVersion,
    ARM_USART_GetCapabilities,
    ARM_USART_Initialize,
    ARM_USART_Uninitialize,
    ARM_USART_PowerControl,
    ARM_USART_Send,
    ARM_USART_Receive,
    ARM_USART_Transfer,
    ARM_USART_GetTxCount,
    ARM_USART_GetRxCount,
    ARM_USART_Control,
    ARM_USART_GetStatus,
    ARM_USART_SetModemControl,
    ARM_USART_GetModemStatus
};

void LPUART0_RxTx_IRQHandler(void)
{
    uart_instance.cb;
}
void LPUART1_RxTx_IRQHandler(void)
{
    uart_instance.cb;
}
void LPUART2_RxTx_IRQHandler(void)
{
    uart_instance.cb;
}



