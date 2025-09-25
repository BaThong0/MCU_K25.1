#include "driver_port.h"
#define PCC ((PCC_Type *)IP_PCC_BASE)

/* Lookup helpers */
static inline uint32_t get_pcc_index(Driver_PortInstance port)
{
	switch (port)
	{
		case DRIVER_PORTA: return PCC_PORTA_INDEX;
		case DRIVER_PORTB: return PCC_PORTB_INDEX;
		case DRIVER_PORTC: return PCC_PORTC_INDEX;
		case DRIVER_PORTD: return PCC_PORTD_INDEX;
		case DRIVER_PORTE: return PCC_PORTE_INDEX;
		default: 		   return PCC_PORTA_INDEX;
	}
}

static inline PORT_Type* get_port_base (Driver_PortInstance port)
{
	switch (port)
	{
		case DRIVER_PORTA: return IP_PORTA;
		case DRIVER_PORTB: return IP_PORTB;
		case DRIVER_PORTC: return IP_PORTC;
		case DRIVER_PORTD: return IP_PORTD;
		case DRIVER_PORTE: return IP_PORTE;
		default: 		   return IP_PORTA;
	}
}

void DRIVER_PORT_EnableClock(Driver_PortInstance port)
{
	uint32_t idx = get_pcc_index(port);
	PCC->PCCn[idx] &= ~PCC_PCCn_CGC_MASK;
	PCC->PCCn[idx] |= PCC_PCCn_CGC_MASK;
}

void DRIVER_PORT_PinMux(Driver_PortInstance port, uint8_t pin, Driver_PortMux mux)
{
	PORT_Type* p = get_port_base(port);
	p->PCR[pin] = (p->PCR[pin] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(mux);
}

void DRIVER_PORT_PullConfig(Driver_PortInstance port,
							uint8_t pin,
							uint8_t enable,
							uint8_t pullup)
{
	PORT_Type* p = get_port_base(port);
	if(enable)
	{
		p->PCR[pin] |= PORT_PCR_PE_MASK;
		if(pullup)	p->PCR[pin] |= PORT_PCR_PS_MASK;
		else 		p->PCR[pin] &= ~PORT_PCR_PS_MASK;
	}
	else
	{
		p->PCR[pin] &= ~PORT_PCR_PE_MASK;
	}
}

/* === Interrupts === */
static Driver_PortCallback callbacks[5] = {0};

void DRIVER_PORT_RegisterCallback(Driver_PortInstance port, Driver_PortCallback cb)
{
	callbacks[port] = cb;
}

void DRIVER_PORT_PinInterruptConfig(Driver_PortInstance port,
									uint8_t pin,
									Driver_PortIrqConfig irqMode)
{
	PORT_Type* p = get_port_base(port);
	p->PCR[pin] = (p->PCR[pin] & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(irqMode);

	/*Enabling corresponding NVIC interrupt */
	IRQn_Type irq;
	switch(port)
	{
		case DRIVER_PORTA: irq = PORTA_IRQn; break;
		case DRIVER_PORTB: irq = PORTB_IRQn; break;
		case DRIVER_PORTC: irq = PORTC_IRQn; break;
		case DRIVER_PORTD: irq = PORTD_IRQn; break;
		case DRIVER_PORTE: irq = PORTE_IRQn; break;
		default: return;
	}
	NVIC_EnableIRQ(irq);
}

void DRIVER_PORT_ClearInterruptFlag(Driver_PortInstance port, uint8_t pin)
{
	PORT_Type* p = get_port_base(port);
	/* Write 1 to clear in ISFR */
	p->ISFR = (1UL << pin);
}

/* === Default ISR handlers ===
 * Each ISR checks ISFR to find active pins and calls user callback
 */

void PORTA_IRQHandler(void)
{
    uint32_t flags = IP_PORTA->ISFR;
    IP_PORTA->ISFR = flags;
    if (callbacks[DRIVER_PORTA]) {
        for (uint8_t i = 0; i < 32; ++i)
            if (flags & (1UL << i)) callbacks[DRIVER_PORTA](i);
    }
}

void PORTB_IRQHandler(void)
{
    uint32_t flags = IP_PORTB->ISFR;
    IP_PORTB->ISFR = flags;
    if (callbacks[DRIVER_PORTB]) {
        for (uint8_t i = 0; i < 32; ++i)
            if (flags & (1UL << i)) callbacks[DRIVER_PORTB](i);
    }
}

void PORTC_IRQHandler(void)
{
    uint32_t flags = IP_PORTC->ISFR;
    IP_PORTC->ISFR = flags;
    if (callbacks[DRIVER_PORTC]) {
        for (uint8_t i = 0; i < 32; ++i)
            if (flags & (1UL << i)) callbacks[DRIVER_PORTC](i);
    }
}

void PORTD_IRQHandler(void)
{
    uint32_t flags = IP_PORTD->ISFR;
    IP_PORTD->ISFR = flags;
    if (callbacks[DRIVER_PORTD]) {
        for (uint8_t i = 0; i < 32; ++i)
            if (flags & (1UL << i)) callbacks[DRIVER_PORTD](i);
    }
}

void PORTE_IRQHandler(void)
{
    uint32_t flags = IP_PORTE->ISFR;
    IP_PORTE->ISFR = flags;
    if (callbacks[DRIVER_PORTE]) {
        for (uint8_t i = 0; i < 32; ++i)
            if (flags & (1UL << i)) callbacks[DRIVER_PORTE](i);
    }
}

/* END OF FILE */







