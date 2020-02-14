#include "MKL46Z4.h"
#include "pit.h"
#include "queue.h"
#include "segmentLCD.h"
#include "uart.h"
#include "tsi.h"

int main(void) {
    SegLCD_Init();
    Init_UART0_IRQ();
    putNum(1001, 16);
}
