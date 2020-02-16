#include "MKL46Z4.h"
#include "pit.h"
#include "queue.h"
#include "segmentLCD.h"
#include "uart.h"
#include "tsi.h"

int main(void) {
    Init_UART0_IRQ();
    SegLCD_Init();
    Init_TSI_IRQ();
    uint8_t barPercentage;
    while(1){
        barPercentage = readPercentage();
        SegLCD_DisplayDecimal(barPercentage);
    }

}
