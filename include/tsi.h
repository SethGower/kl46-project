#ifndef TSI_H
#define TSI_H
#include "MKL46Z4.h"

#define TSI_GENCS_TSI_TSII_EN TSI_GENCS_TSIIEN_MASK | TSI_GENCS_TSIEN_MASK

void TSI0_IRQHandler(void);
void Init_TSI_IRQ(void);
uint16_t getTSIPosition(void);

#endif
