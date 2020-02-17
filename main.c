#include "MKL46Z4.h"
#include "pit.h"
#include "queue.h"
#include "segmentLCD.h"
#include "tsi.h"
#include "uart.h"

void delay(void) {
    volatile unsigned int i, j;

    for (i = 0U; i < 50000U; i++) {
        for (j = 0U; j < 100U; j++) {
            __asm__("nop");
        }
    }
}
void gpio_init() {
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
    PORTC->PCR[3] |= PORT_PCR_MUX(1u) | PORT_PCR_IRQC(10) | PORT_PCR_PE_MASK |
                     PORT_PCR_PS_MASK;
    PORTE->PCR[29] |= PORT_PCR_MUX(1u) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

}
int main(void) {
    Init_UART0_IRQ();
    gpio_init();
    putNum(PTC->PDDR, 32, 2);
    putString("\r\n");
    PTE->PDOR = 1u << 3;

    while (1) {
        putNum(PTC->PDDR, 32, 2);
        putString("\r\n");
        /* putNum(PTC->PDIR, 32, 2); */
        /* putString("\r\n"); */
        delay();
    }
}
