#include "tsi.h"
#include "MKL46Z4.h"

static uint32_t value;
void Init_TSI_IRQ(void) {
    /* select clock */
    SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;
    /* enable IRQ */
    NVIC_SetPriority(TSI0_IRQn,3);
    NVIC_ClearPendingIRQ(TSI0_IRQn);
    NVIC_EnableIRQ(TSI0_IRQn);
    TSI0->GENCS = TSI_GENCS_TSI_TSII_EN;
}
void TSI0_IRQHandler(void) {
    value = TSI0->DATA;
}
uint16_t getTSIPosition(){
    return value;
}
