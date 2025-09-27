#include "nvic_custom.h"

void NVIC_EnableIRQ(IRQn_Type IRQn)
{
	if(IRQn >= 0)
	{
		uint32_t index = ((uint32_t)IRQn) >> 5U;
		uint32_t bit = 1UL << ((uint32_t)IRQn & 0x1FUL);
		NVIC->ISER[index] = bit;
		__asm volatile ("dsb");
		__asm volatile ("isb");
	}
}

void NVIC_DisableIRQ(IRQn_Type IRQn)
{
    if (IRQn >= 0) {
        uint32_t index = ((uint32_t)IRQn) >> 5U;
        uint32_t bit   = 1UL << ((uint32_t)IRQn & 0x1FUL);
        NVIC->ICER[index] = bit;
        __asm volatile("dsb");
        __asm volatile("isb");
    }
}

void NVIC_SetPriority(IRQn_Type IRQn, uint8_t priority)
{
    if (IRQn >= 0) {
        NVIC->IP[(uint32_t)IRQn] = priority & 0xFFU;
    }
}
