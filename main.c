#include "MKL46Z4.h"
#include "gpio.h"
#include "pit.h"
#include "queue.h"
#include "segmentLCD.h"
#include "tsi.h"
#include "uart.h"

int main(void) {
    Init_UART0_IRQ();
    Init_PIT_IRQ();
    SegLCD_Init();
    gpio_init();
}
