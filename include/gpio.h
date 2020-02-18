#ifndef GPIO_H
#define GPIO_H
#include "MKL46Z4.h"
#include "uart.h"

#define SW1 3
void gpio_init(void);
void GPIO_IRQHandler(void);

#endif
