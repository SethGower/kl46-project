/******************************************************************************
 * File:             tsi.c
 *
 * Author:           Seth Gower
 * Created:          02/15/20
 * Description:      This is a driver for the KL46 boards TSI component, this
 *                   driver was adapted from the .cpp driver located at
 *                   https://os.mbed.com/teams/Multi-Hackers/code/FRDM-KL46-Template/
 *                   for use in C
 *****************************************************************************/

#include "tsi.h"
#include "MKL46Z4.h"

static uint8_t total_electrode = TOTAL_ELECTRODE;

static uint8_t elec_array[16] = {
    ELECTRODE0,  ELECTRODE1,  ELECTRODE2,  ELECTRODE3, ELECTRODE4,  ELECTRODE5,
    ELECTRODE6,  ELECTRODE7,  ELECTRODE8,  ELECTRODE9, ELECTRODE10, ELECTRODE11,
    ELECTRODE12, ELECTRODE13, ELECTRODE14, ELECTRODE15};
static uint16_t gu16TSICount[16];
static uint16_t gu16Baseline[16];
static uint16_t gu16Threshold[16] = {
    THRESHOLD0,  THRESHOLD1,  THRESHOLD2,  THRESHOLD3, THRESHOLD4,  THRESHOLD5,
    THRESHOLD6,  THRESHOLD7,  THRESHOLD8,  THRESHOLD9, THRESHOLD10, THRESHOLD11,
    THRESHOLD12, THRESHOLD13, THRESHOLD14, THRESHOLD15};
static uint16_t gu16Delta[16];
static uint8_t ongoing_elec;
static uint8_t end_flag = 1;

static uint8_t SliderPercentegePosition[2] = {NO_TOUCH, NO_TOUCH};
static uint8_t SliderDistancePosition[2] = {NO_TOUCH, NO_TOUCH};
static uint32_t AbsolutePercentegePosition = NO_TOUCH;
static uint32_t AbsoluteDistancePosition = NO_TOUCH;

void Init_TSI_IRQ(void) {
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;

    TSI0->GENCS |=
        (TSI_GENCS_ESOR_MASK | TSI_GENCS_MODE(0) | TSI_GENCS_REFCHRG(4) |
         TSI_GENCS_DVOLT(0) | TSI_GENCS_EXTCHRG(7) | TSI_GENCS_PS(4) |
         TSI_GENCS_NSCN(11) | TSI_GENCS_TSIIEN_MASK | TSI_GENCS_STPE_MASK);

    TSI0->GENCS |= TSI_GENCS_TSIEN_MASK;

    NVIC_SetPriority(TSI0_IRQn, 3);
    NVIC_ClearPendingIRQ(TSI0_IRQn);
    NVIC_EnableIRQ(TSI0_IRQn);
    selfCalibration();
}
void TSI0_IRQHandler(void) {
    end_flag = 1;
    TSI0->GENCS |= TSI_GENCS_EOSF_MASK; // Clear End of Scan Flag
    changeElectrode();
}
void selfCalibration(void) {
    unsigned char cnt;
    unsigned char trigger_backup;

    TSI0->GENCS |= TSI_GENCS_EOSF_MASK;   // Clear End of Scan Flag
    TSI0->GENCS &= ~TSI_GENCS_TSIEN_MASK; // Disable TSI module

    if (TSI0->GENCS &
        TSI_GENCS_STM_MASK) // Back-up TSI Trigger mode from Application
        trigger_backup = 1;
    else
        trigger_backup = 0;

    TSI0->GENCS &= ~TSI_GENCS_STM_MASK;    // Use SW trigger
    TSI0->GENCS &= ~TSI_GENCS_TSIIEN_MASK; // Enable TSI interrupts

    TSI0->GENCS |= TSI_GENCS_TSIEN_MASK; // Enable TSI module

    for (cnt = 0; cnt < total_electrode;
         cnt++) // Get Counts when Electrode not pressed
    {
        TSI0->DATA = ((elec_array[cnt] << TSI_DATA_TSICH_SHIFT));
        TSI0->DATA |= TSI_DATA_SWTS_MASK;
        while (!(TSI0->GENCS & TSI_GENCS_EOSF_MASK))
            ;
        TSI0->GENCS |= TSI_GENCS_EOSF_MASK;
        gu16Baseline[cnt] = (TSI0->DATA & TSI_DATA_TSICNT_MASK);
    }

    TSI0->GENCS &= ~TSI_GENCS_TSIEN_MASK; // Disable TSI module
    TSI0->GENCS |= TSI_GENCS_TSIIEN_MASK; // Enale TSI interrupt
    if (trigger_backup)                   // Restore trigger mode
        TSI0->GENCS |= TSI_GENCS_STM_MASK;
    else
        TSI0->GENCS &= ~TSI_GENCS_STM_MASK;

    TSI0->GENCS |= TSI_GENCS_TSIEN_MASK; // Enable TSI module

    TSI0->DATA = ((elec_array[0] << TSI_DATA_TSICH_SHIFT));
    TSI0->DATA |= TSI_DATA_SWTS_MASK;
}

void changeElectrode() {

    int16_t u16temp_delta;
    /* Save Counts for current electrode */
    gu16TSICount[ongoing_elec] = (TSI0->DATA & TSI_DATA_TSICNT_MASK);

    /* Obtains counts delta from callibration reference */
    u16temp_delta = gu16TSICount[ongoing_elec] - gu16Baseline[ongoing_elec];

    if (u16temp_delta < 0)
        gu16Delta[ongoing_elec] = 0;
    else
        gu16Delta[ongoing_elec] = u16temp_delta;

    /* Change Electrode to Scan */
    if (total_electrode > 1) {
        if ((total_electrode - 1) > ongoing_elec)
            ongoing_elec++;
        else
            ongoing_elec = 0;

        TSI0->DATA = ((elec_array[ongoing_elec] << TSI_DATA_TSICH_SHIFT));
        TSI0->DATA |= TSI_DATA_SWTS_MASK;
    }
}

void slideRead() {
    if (end_flag) {
        end_flag = 0;
        if ((gu16Delta[0] > gu16Threshold[0]) ||
            (gu16Delta[1] > gu16Threshold[1])) {
            SliderPercentegePosition[0] =
                (gu16Delta[0] * 100) / (gu16Delta[0] + gu16Delta[1]);
            SliderPercentegePosition[1] =
                (gu16Delta[1] * 100) / (gu16Delta[0] + gu16Delta[1]);
            SliderDistancePosition[0] =
                (SliderPercentegePosition[0] * SLIDER_LENGTH) / 100;
            SliderDistancePosition[1] =
                (SliderPercentegePosition[1] * SLIDER_LENGTH) / 100;
            AbsolutePercentegePosition = ((100 - SliderPercentegePosition[0]) +
                                          SliderPercentegePosition[1]) /
                                         2;
            AbsoluteDistancePosition =
                ((SLIDER_LENGTH - SliderDistancePosition[0]) +
                 SliderDistancePosition[1]) /
                2;
        }
    }
}
uint8_t readDistance(void) {
    slideRead();
    return AbsoluteDistancePosition;
}
uint8_t readPercentage(void) {
    slideRead();
    return AbsolutePercentegePosition;
}
