#include "pit.h"
#include <MKL46Z4.h>
#include <stdint.h>

static uint8_t runStopwatch;
static uint32_t count;
void Init_PIT_IRQ(void) {
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;             /* enables clock for PIT */
    PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK; /* disables timer */
    NVIC_SetPriority(PIT_IRQn, 3);                /* sets interrupt priority */
    NVIC_ClearPendingIRQ(PIT_IRQn);               /* clear pending interrupts */
    NVIC_EnableIRQ(PIT_IRQn);                     /* enables interrupt */
    PIT->MCR = PIT_MCR_FRZ_MASK;                  /* enables debug freezing */
    setCountValue(10);                            /* sets initial value 10ms */
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_CH_IE;      /* enables timer interrupts */
}
void PIT_IRQHandler(void) {
    __asm("CPSID I");
    if (runStopwatch)
        count++;
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
    __asm("CPSIE I");
}

void enableTimer(void) { runStopwatch = 1; }
void disableTimer(void) { runStopwatch = 0; }
uint32_t getTimer(void) { return count; }
void setCountValue(uint32_t milliseconds) {
    if (milliseconds > HIGHEST_MS)
        milliseconds = HIGHEST_MS - 100;
    uint32_t value = milliseconds * 23490;
    PIT->CHANNEL[0].LDVAL = value;
}
