#ifndef ADC_JOY_H
#define ADC_JOY_H

#include "MKL05Z4.h"

void ADC_Init(void);
uint16_t ADC_Read(int channel);

#endif