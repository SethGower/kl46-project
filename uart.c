#include "uart.h"
#include "MKL46Z4.h"
#include "queue.h"
#include "segmentLCD.h"
queue_t RxRecord = {0, 0, 0, 0, 0, 0};
queue_t TxRecord = {0, 0, 0, 0, 0, 0};

char RxBuffer[TRxQSize];
char TxBuffer[TRxQSize];

void Init_UART0_IRQ(void) {
    /* Initializes the Queues to use for interrupt based communication */
    initQueue(&RxRecord, RxBuffer, TRxQSize);
    initQueue(&TxRecord, TxBuffer, TRxQSize);

    /* Select MCGPLLCLK / 2 as UART0 Clock Source */
    SIM->SOPT2 &= ~SIM_SOPT2_UART0SRC_MASK;       /* disables clock */
    SIM->SOPT2 |= SIM_SOPT2_UART0_MCGPLLCLK_DIV2; /* enables PLL */

    /* Enable External Connection for UART0 */
    /* Clears the TX Source to conect to UART0_TX pin
     * Clears UART0 RX Source to connect to UART0_RX pin
     * Disables th open drain for UART0*/
    SIM->SOPT5 &= ~SIM_SOPT5_UART0_EXTERN_MASK_CLEAR;

    /* Enable clock for UART0 module */
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;

    /* Enable clock for port A */
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;

    /* Connect Port A pin 1 to UART0 Rx */
    PORTA->PCR[1] = PORT_PCR_SET_PTA_UART0_TR;

    /* Connect Port A pin 2 to UART0 Tx */
    PORTA->PCR[2] = PORT_PCR_SET_PTA_UART0_TR;

    /* Disable UART0 Receiver and Transmitter */
    UART0->C2 &= ~UART0_C2_T_R;

    /* Set Interrupt for UART0 */

#if 1
    NVIC->IP[3] |= NVIC_IPR_UART0_MASK;    /* Sets priority to level 3 */
    NVIC->ICPR[0] |= NVIC_ICPR_UART0_MASK; /* Clears the Interrupts to UART0 */
    NVIC->ISER[0] |= NVIC_ISER_UART0_MASK; /* Unmask UART0 interrupts */
#else
    NVIC_SetPriority(UART0_IRQn, 3);
    NVIC_ClearPendingIRQ(UART0_IRQn);
    NVIC_EnableIRQ(UART0_IRQn);
#endif

    /* Sets UART0 to 9600, 8N1 */
    UART0->BDH = UART0_BDH_9600; /* Sets the High byte of 9600 baud */
    UART0->BDL = UART0_BDL_9600; /* sets the low byte of 9600 baud*/
    UART0->C1 = UART0_8N1;       /* sets the 8N1 mode */
    UART0->C3 = UART0_C3_NO_TXINV;
    UART0->C4 = UART0_C4_NO_MATCH_OSR_16;
    UART0->C5 = UART0_C5_NO_DMA_SSR_SYNC;
    UART0->S1 = UART0_S1_CLEAR_FLAGS;
    UART0->S2 = UART0_S2_NO_RXINV_BRK10_NO_LBKDETECT_CLEAR_FLAGS;
    UART0->C2 = UART0_C2_T_RI;
}
void UART0_IRQHandler(void) {
    __asm("CPSID I");
    /* checks if a transmit interrupt has been requested */
    if (UART0->C2 & UART0_C2_TIE_MASK) { 
        /* checks if the transmit buffer is empty */
        if (UART0->S1 & UART0_S1_TDRE_MASK) { 
            if (dequeue((char *)(&UART0->D), &TxRecord) == EXIT_FAILURE){
                UART0->C2 = UART0_C2_T_RI;
            }
        }
    }
    /* checks if the receive buffer is full */
    if (UART0->S1 & UART0_S1_RDRF_MASK) {
        enqueue(UART0->D, &RxRecord);
    }
    __asm("CPSIE I");
}
void putChar(char c) {
    int success = 0;
    do {
        __asm("CPSID I");
        success = enqueue(c, &TxRecord);
        __asm("CPSIE I");
    } while (success);
    /* requests the interrupt */
	UART0->C2 = UART0_C2_TI_RI;
}
char getChar() {
    char character;
    int success = 0;
    do {
        __asm("CPSID I");
        success = dequeue(&character, &RxRecord);
        __asm("CPSIE I");
    } while (success);

    return character;
}
