#include "main.h"
#include "MKL46Z4.h"
#include "segmentLCD.h"
#include "queue.h"
#include <errno.h>

void delay(void);
int main(void) {
    char buff[20];
    queue_t record = {0};
    SegLCD_Init();
    initQueue(buff, &record, 20);
}
void delay(void) {
    volatile unsigned int i, j;

    for (i = 0U; i < 5000U; i++) {
        for (j = 0U; j < 100U; j++) {
            __asm__("nop");
        }
    }
}
