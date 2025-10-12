/**
 * @file    main.c
 * @author  Vo Ba Thong
 * @brief   Application entry point.
 * @details Control Led colors based on potentiometer through ADC peripheral
 */

#include "S32K144.h"
#include "clocks_and_modes.h"
#include "ADC.h"

#define PTD15 15 /* LED RED */
#define PTD16 16 /* LED GREEN */
#define PTD0 0   /* LED BLUE */

uint32_t adcResultInMv_pot = 0;
uint32_t adcResultInMv_Vrefsh = 0;

void GPIO_init(void) {
    /* Enable clock for PORTD */
    IP_PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK;

    /* Set PTD0, PTD15, PTD16 as GPIO output */
    IP_PORTD->PCR[PTD0]  = PORT_PCR_MUX(1);
    IP_PORTD->PCR[PTD15] = PORT_PCR_MUX(1);
    IP_PORTD->PCR[PTD16] = PORT_PCR_MUX(1);

    /* Port D0: Data Direction = output */
    IP_PTD->PDDR |= 1 << PTD0;
    /* Port D15: Data Direction = output */
    IP_PTD->PDDR |= 1 << PTD15;
    /* Port D16: Data Direction = output */
    IP_PTD->PDDR |= 1 << PTD16;
}

int main(void) {
    
    GPIO_init();
    ADC_init();

    while (1) {
        /* Convert channel AD12 to port on EVB */
        convertADCchan(12);
        /* Wait for conversion complete flag */
        while(adc_complete() == 0)
        {
            /*Do nothing*/
        }
        /* Get channel's conversion results in mV */
        adcResultInMv_pot = read_ADC_chx();

        if(adcResultInMv_pot > 3750)        /* If result > 3750mV */
        {
            /* turn off blue, green LEDs */
            IP_PTD->PSOR |= 1<<PTD0 | 1<<PTD16;
            /* turn on red LED */
            IP_PTD->PCOR |= 1<<PTD15; 
        } else if(adcResultInMv_pot > 2500) /* If result > 2500mV */
        {
            /* turn off blue, red LEDs */
            IP_PTD->PSOR |= 1<<PTD0 | 1<<PTD15;
            /* turn on green LED */
            IP_PTD->PCOR |= 1<<PTD16;
        } else if(adcResultInMv_pot > 1250) /* If result > 1250mV */
        {
            /* turn off red, green LEDs */
            IP_PTD->PSOR |= 1<<PTD15 | 1<<PTD16;
            /* turn on blue LED */
            IP_PTD->PCOR |= 1<<PTD0;
        } else 
        {
            /* Turn off all LEDs */
            IP_PTD->PSOR |= 1<<PTD0 | 1<<PTD15 | 1<<PTD16;
        }
        /* Convert channel 29, Vrefsh */
        convertADCchan(29);
        /* Wait for conversion complete flag */
        while(adc_complete() == 0)
        {
            /* Do nothing */
        }
        /* Get channel's conversion results in mV */
        adcResultInMv_Vrefsh = read_ADC_chx();
    }

}
