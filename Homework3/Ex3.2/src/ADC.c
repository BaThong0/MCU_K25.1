#include "ADC.h"

/**
 * @brief Initialize the ADC peripheral
 * 
 */
void ADC_init(void)
{
    /* Disable clock to change PCS */
    IP_PCC->PCCn[PCC_ADC0_INDEX] &= ~PCC_PCCn_CGC_MASK;
    /* PCS = 1: Select SOSCDIV2 */
    IP_PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_PCS(1);
    /* Enable bus clock in ADC */
    IP_PCC->PCCn[PCC_ADC0_INDEX] |= PCC_PCCn_CGC_MASK;

    /* Enable clock for port C */
    IP_PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC_MASK;

    /* Config PTC14 -> ADC0_SE12 */
    IP_PORTC->PCR[14] = 0;
    
    /* ADCH=1F: Module is disabled for conversions */
    /* AIEN=0: Interrupts are disabled */
    IP_ADC0->SC1[0] = 0x00001F;
    /* ADICLK=0: Input clk=ALTCLK1=SOSCDIV2 */
    /* ADIV=0: Prescaler=1 */
    /* MODE=1: 12-bit conversion */
    IP_ADC0->CFG1 = 0x000000004;
    /* SMPLTS=12(default): sample time is 13 ADC clks */
    IP_ADC0->CFG2 = 0x00000000C;
    /* ADTRG=0: SW trigger */
    /* ACFE, ACFGT, ACREN=0: Compare func disabled */
    /* DMAEN=0: DMA disabled */
    /* REFSEL=0: Voltage reference pins= VREFH, VREEFL*/
    IP_ADC0->SC2 = 0x00000000;
    /* CAL=0: Do not start calibration sequence */
    /* ADC0=0: One conversion performed */
    /* AVGE, AVGS=0: HW average function disabled */
    IP_ADC0->SC3 = 0x00000000;

}

/**
 * @brief start an ADC channel
 * 
 * @param adcChan 
 */
void convertADCchan(uint16_t adcChan)
{
    /* Clear prior ADCH bits */
    IP_ADC0->SC1[0] &= ~ADC_SC1_ADCH_MASK;
    /* Initiate Conversion */
    IP_ADC0->SC1[0] = ADC_SC1_ADCH(adcChan); 
}

/**
 * @brief wait for complete transfer data
 * 
 * @return uint8_t 
 */
uint8_t adc_complete(void)
{
    /* Wait for completion */
    return ((IP_ADC0->SC1[0] & ADC_SC1_COCO_MASK) >> ADC_SC1_COCO_SHIFT);
}

/**
 * @brief read value from ADC channel
 * 
 * @return uint32_t 
 */
uint32_t read_ADC_chx(void)
{
    uint16_t adc_result = 0;
    /* For SW trigger mode, R[0] is used */
    adc_result = IP_ADC0->R[0];
    /* Convert result to mV for 0-5V range */
    return (uint32_t) ((5000*adc_result)/0xFFF);
}
