#include "S32K144.h"
#include "../Core/Include/core_cm4.h"
#include "clocks_and_modes.h"

/* main loop idle counter */
int idle_counter = 0;
/* LPIT0 channel 0 timeout counter */
int lpit0_ch0_flag_counter = 0;

/* Assume LED is on PTD15 (already configured as GPIO output elsewhere) */
void GPIO_Init(void) 
{
    *(uint32_t *)0x40065130 |= (1u << 30); // PCC GPIOD
    *(uint32_t *)0x4004C000 |= (1u << 8);  // PORTD0 MUX = GPIO
    *(uint32_t *)0x4004C03C |= (1u << 8);  // PORTD15 MUX = GPIO
    *(uint32_t *)0x4004C040 |= (1u << 8);  // PORTD16 MUX = GPIO
    *(uint32_t *)0x400FF0D4 |= (1u << 0);  // GPIOD0 as output
    *(uint32_t *)0x400FF0D4 |= (1u << 15); // GPIOD15 as output
    *(uint32_t *)0x400FF0D4 |= (1u << 16); // GPIOD16 as output
    *(uint32_t *)0x400FF0C4 |= (1u << 0);  // set pin0 high
    *(uint32_t *)0x400FF0C4 |= (1u << 15); // set pin15 high
    *(uint32_t *)0x400FF0C4 |= (1u << 16); // set pin16 high
}

void NVIC_init(void) {
    /* Clear any pending IRQ */
    NVIC->ICPR[1] = 1U << (LPIT0_Ch0_IRQn % 32);
    /* Enable IRQ */
    NVIC->ISER[1] = 1U << (LPIT0_Ch0_IRQn % 32);
    /* Set priority IRQ */
    NVIC->IPR[48] = 0x0A;
}

void LPIT0_init(void) {
    /* Clock Src = SPLL2_DIV2_CLK */
    IP_PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);
    /* Enable clock to LPIT0 regs */ 
    IP_PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK;
    /* DBG_EN-0: Timer chans stop in Debug mode */
    /* DOZE_EN=0: Timer chans are stopped in DOZE mode */
    /* SW_RST=0: SW reset does not reset timer chans, regs */
    /* M_CEN=1: enable module clk (allow writing other LPIT0 regs) */
    IP_LPIT0->MCR = 0x01;
    /* ITE0 = 1: Timer interrupt enable for channel 0 */
    IP_LPIT0->MIER = 0x01;
    /* Channel 0 timeout period: 80M clocks */
    IP_LPIT0->TMR[0].TVAL = 120000000;
    /* T_EN=1: Timer channel is enabled */
    /* CHAIN=0: channel chaining is disabled */
    /* MODE=0: 32 periodic counter mode */
    /* TSOT=0: Timer decrements immediately based on restart */
    /* TSOI=0: Timer does not stop after timeout */
    /* TROT=0 Timer will not reload on trigger */
    /* TRG_SRC=0: External trigger source */
    /* TRG_SEL=0: Timer chan 0 trigger source is selected*/
    IP_LPIT0->TMR[0].TCTRL = 0x01;

}

int main(void) 
{
    /* Configure GPIO */
    GPIO_Init();
    /* Initialize system oscillator for 8 MHz xtal */
    SOSC_init_8MHz();
    /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
    SPLL_init_160MHz();
    /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */
    NormalRUNmode_80MHz();
    /* Enable desired interrupts and priorities */
    NVIC_init();
    /* --- LPIT initialization --- */
    LPIT0_init();

    while (1) {
        idle_counter++;
    }
}

void LPIT0_Ch0_IRQHandler (void) 
{
    /* Clear LPIT0 Timer flag 0 */
    IP_LPIT0->MSR |= LPIT_MSR_TIF0_MASK;
    /* Perform read-after-write to ensure flag clears before ISR exit */
    /* Increment LPIT0 timeout counter */
    lpit0_ch0_flag_counter++;
    /* Toggle output on port D0 */
    IP_PTD->PTOR |= 1 << 0;
}
