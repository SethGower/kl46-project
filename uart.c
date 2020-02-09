#include "uart.h"
#include "MKL46Z4.h"
#include "queue.h"
#include <errno.h>
static queue_t *RxRecord;
static queue_t *TxRecord;

static char RxBuffer[40];
static char TxBuffer[40];

void Init_UART0_IRQ(void) {
    /* Initializes the Queues to use for interrupt based communication */
    initQueue(RxBuffer, RxRecord, TRxQSize);
    initQueue(TxBuffer, TxRecord, TRxQSize);

    /* Select MCGPLLCLK / 2 as UART0 Clock Source */
    SIM->SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_UART0_MCGPLLCLK_DIV2;

    /* Enable External Connection for UART0 */
    SIM->SOPT5 &= ~SIM_SOPT5_UART0_EXTERN_MASK_CLEAR;

    /* Enable clock for UART0 module */
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;

    /* Enable clock for port A */
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

    /* Connect Port A pin 1 to UART0 Rx */
    PORTA->PCR[1] = PORT_PCR_SET_PTA1_UART0_RX;

    /* Connect Port A pin 2 to UART0 Tx */
    PORTA->PCR[2] = PORT_PCR_SET_PTA2_UART0_TX;

    /* Disable UART0 Receiver and Transmitter */
    UART0->C2 &= ~UART0_C2_T_R;

    /* Set Interrupt for UART0 */
    NVIC->IP[3] |= NVIC_IPR_UART0_MASK;    /* Sets priority to level 3 */
    NVIC->ICPR[0] |= NVIC_ICPR_UART0_MASK; /* Clears the Interrupts to UART0 */
    NVIC->ISER[0] |= NVIC_ICPR_UART0_MASK; /* Unmask UART0 interrupts */
    UART0->BDH = UART0_BDH_9600;           /* Sets the High byte of 9600 baud */
    UART0->BDL = UART0_BDL_9600;           /* sets the low byte of 9600 baud*/
    UART0->C1 = UART0_8N1;
    UART0->C3 = UART0_C3_TXINV;
    UART0->C4 = UART0_C4_NO_MATCH_OSR_16;
    UART0->C5 = UART0_C5_NO_DMA_SSR_SYNC;
    UART0->S1 = 0;
    UART0->S2 = 0x90;
    UART0->C2 |= UART0_C2_T_R;
}
void UART0_ISR(void) {
    char tmp = 0;
    if (!(UART0->C2 & UART_C2_TIE_MASK) && !(UART0->S1 & UART0_S1_TDRE_MASK)) {
    rx:
        if (!(UART0->S1 & UART0_S1_RDRF_MASK)) {
            return;
        } else {
            tmp = UART0->D;
            enqueue(tmp, RxRecord);
        }
    } else {
        tmp = dequeue(TxRecord);
        if (errno) {
            UART0->C2 = UART0_C2_T_R;
        } else {
            goto rx;
        }
    }
}
void putChar(char c) {
    do {
        enqueue(c, TxRecord);
    } while (errno);
}

char getChar(void) {
    char tmp = 0;
    do {
        tmp = dequeue(RxRecord);
    } while (errno);
    return tmp;
}
