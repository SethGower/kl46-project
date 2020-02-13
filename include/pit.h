#ifndef PIT_H
#define PIT_H

#include "MKL46Z4.h"
#include <stdint.h>

#define HIGHEST_MS 182900 /* found experimentally */
#define PIT_TCTRL_CH_IE (PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK)
void Init_PIT_IRQ(void);
void PIT_IRQHandler(void);

void enableTimer(void);
void disableTimer(void);
uint32_t getTimer(void);
void setCountValue(uint32_t milliseconds);
#endif
