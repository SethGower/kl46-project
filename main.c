#include "MKL46Z4.h"
#include "queue.h"
#include "segmentLCD.h"
#include "uart.h"

int main(void) {
    SegLCD_Init();
    Init_UART0_IRQ();
    putChar('F');
}
