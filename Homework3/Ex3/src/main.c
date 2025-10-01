/**
 * @file    main.c
 * @author  Vo Ba Thong
 * @brief   Application entry point.
 * @details Control Led colors based on potentiometer through ADC peripheral
 */

#include "S32K144.h"

void ADC0_init(void) {
    /* Enable clock for PORTC and ADC0 */
    IP_PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;
    IP_PCC->PCCn[PCC_ADC0_INDEX]  = PCC_PCCn_PCS(1) | PCC_PCCn_CGC_MASK;
    // PCS=1: SOSCDIV2_CLK (default ~8MHz)

    /* Configure PTC12 as ADC0_SE12 (analog) */
    IP_PORTC->PCR[12] &= ~PORT_PCR_MUX_MASK;   // MUX = 0 = analog

    /* Configure ADC0: 12-bit single-ended, bus clock/2, long sample */
    IP_ADC0->CFG1 = ADC_CFG1_ADICLK(0)  // Input clk = busclk
               | ADC_CFG1_MODE(1)    // 12-bit mode
               | ADC_CFG1_ADIV(1);   // clk/2 divider

    IP_ADC0->SC1[0] = ADC_SC1_ADCH(1); // Disable module (ADCH=11111)
}

uint16_t ADC0_read(uint8_t channel) {
    IP_ADC0->SC1[0] = channel;                 // Start conversion on channel
    while (!(IP_ADC0->SC1[0] & ADC_SC1_COCO_MASK)); // Wait for complete
    return IP_ADC0->R[0];                      // Read result
}

void GPIO_init(void) {
    /* Enable clock for PORTD */
    IP_PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;

    /* Set PTD0, PTD15, PTD16 as GPIO output */
    IP_PORTD->PCR[0]  = PORT_PCR_MUX(1);
    IP_PORTD->PCR[15] = PORT_PCR_MUX(1);
    IP_PORTD->PCR[16] = PORT_PCR_MUX(1);

    IP_PTD->PDDR |= (1<<0) | (1<<15) | (1<<16);  // output
}

void set_LED(uint8_t r, uint8_t g, uint8_t b) {
    if(r)
    {
    	IP_PTD->PCOR = (1<<0);
    }
    else
    {
    	IP_PTD->PSOR = (1<<0);
    }
    if(g)
	{
    	 IP_PTD->PCOR = (1<<16);
	}
    else
    {
    	IP_PTD->PSOR = (1<<16);
    }
    if(b)
    {
    	IP_PTD->PCOR = (1<<15);

    }
    else
    {
    	IP_PTD->PSOR = (1<<15);
    }
}

int main(void) {
    uint16_t adc_val;
    uint32_t mv;

    GPIO_init();
    ADC0_init();

    while (1) {
        adc_val = ADC0_read(12);  // channel ADC0_SE12 (PTC12)
        mv = (adc_val * 5000U) / 4096U;  // scale to mV (0..5000)

        if (mv < 1250) {
            set_LED(0,0,0);   // OFF
        } else if (mv < 2500) {
            set_LED(0,0,1);   // BLUE
        } else if (mv < 3750) {
            set_LED(0,1,0);   // GREEN
        } else {
            set_LED(1,0,0);   // RED
        }
    }
}
