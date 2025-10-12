#include "S32K144.h"

void ADC_init(void);
void convertADCchan(uint16_t);
uint8_t adc_complete(void);
uint32_t read_adc_chx(void);