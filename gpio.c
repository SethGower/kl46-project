#include "gpio.h"
#include "MKL46Z4.h"
#include "pit.h"
#include "segmentLCD.h"

static int counter = 0;
void delay(void) {
    volatile unsigned int i, j;

    for (i = 0U; i < 10000U; i++) {
        for (j = 0U; j < 100U; j++) {
            __asm__("nop");
        }
    }
}
void gpio_init(void) {
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK; /* enable clock for PORTC */
    PTC->PDDR &= ~(1u << SW1);          /* sets ptc3 low for input */
    PORTC->PCR[SW1] = PORT_PCR_MUX(1u) | PORT_PCR_IRQC(8);
    PORTC->PCR[SW1] |= PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
    putString("\r\nPCR\r\n");
    putNum(PORTC->PCR[SW1], 32, 2);
    putString("\r\nPDDR\r\n");
    putNum(PTC->PDDR, 32, 2);
    putString("\r\nPDIR\r\n");
    putNum(PTC->PDIR, 32, 2);
    NVIC_SetPriority(PORTD_IRQn, 3);
    NVIC_ClearPendingIRQ(PORTD_IRQn);
    NVIC_EnableIRQ(PORTD_IRQn);
    counter = 1;
}
void GPIO_IRQHandler(void) {
    uint32_t PDIR = PTC->PDIR;
    if (!PDIR) {
        enableTimer();
        SegLCD_DisplayDecimal(counter++);
        putChar('f');
        while(getTimer() < 20){}
        disableTimer();
        resetTimer();
    }

    NVIC_ClearPendingIRQ(PORTD_IRQn);
}
