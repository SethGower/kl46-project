#ifndef TSI_H
#define TSI_H
#include "MKL46Z4.h"

#define TSI_GENCS_TSI_TSII_EN TSI_GENCS_TSIIEN_MASK | TSI_GENCS_TSIEN_MASK

#define NO_TOUCH 0
#define SLIDER_LENGTH 40 // LENGTH in mm
#define TOTAL_ELECTRODE 2

#define TSI0a 0
#define TSI1 1
#define TSI2 2
#define TSI3 3
#define TSI4 4
#define TSI5 5
#define TSI6 6
#define TSI7 7
#define TSI8 8
#define TSI9 9
#define TSI10 10
#define TSI11 11
#define TSI12 12
#define TSI13 13
#define TSI14 14
#define TSI15 15

/*Chose the correct TSI channel for the electrode number*/
#define ELECTRODE0 TSI9
#define ELECTRODE1 TSI10
#define ELECTRODE2 TSI0a
#define ELECTRODE3 TSI1
#define ELECTRODE4 TSI2
#define ELECTRODE5 TSI3
#define ELECTRODE6 TSI4
#define ELECTRODE7 TSI5
#define ELECTRODE8 TSI6
#define ELECTRODE9 TSI7
#define ELECTRODE10 TSI8
#define ELECTRODE11 TSI11
#define ELECTRODE12 TSI12
#define ELECTRODE13 TSI13
#define ELECTRODE14 TSI14
#define ELECTRODE15 TSI15

#define THRESHOLD0 100
#define THRESHOLD1 100
#define THRESHOLD2 100
#define THRESHOLD3 100
#define THRESHOLD4 100
#define THRESHOLD5 100
#define THRESHOLD6 100
#define THRESHOLD7 100
#define THRESHOLD8 100
#define THRESHOLD9 100
#define THRESHOLD10 100
#define THRESHOLD11 100
#define THRESHOLD12 100
#define THRESHOLD13 100
#define THRESHOLD14 100
#define THRESHOLD15 100

void TSI0_IRQHandler(void);
void Init_TSI_IRQ(void);
uint16_t getTSIPosition(void);
uint8_t readDistance(void);
uint8_t readPercentage(void);
void selfCalibration(void);
void changeElectrode();

#endif
