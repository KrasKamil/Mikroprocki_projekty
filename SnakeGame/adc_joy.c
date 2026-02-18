#include "adc_joy.h"

void ADC_Init(void) {
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
    ADC0->CFG1 = ADC_CFG1_MODE(1); // 12-bit
}

uint16_t ADC_Read(int channel) {
    ADC0->SC1[0] = channel;
    while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));
    return ADC0->R[0];
}