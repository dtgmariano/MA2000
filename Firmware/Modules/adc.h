/*****************************************************************************
 *   adc.h:  Header file for Philips LPC214x Family Microprocessors
 *
 *   Copyright(C) 2006, Philips Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2005.10.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __ADC_H
#define __ADC_H
#include <stdint.h>

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

#define ADC_INTERRUPT_FLAG	0	/* 1 is interrupt driven, 0 is polling */

#define ADC_OFFSET	0x10
#define ADC_INDEX	4

#define ADC_DONE	0x80000000
#define ADC_OVERRUN	0x40000000
#define ADC_ADINT	0x00010000

#define ADC_NUM		8		/* for LPC2146/8 */
#define ADC_CLK		1000000		/* set to 1Mhz */

uint32_t ADC_Init( uint32_t ADC_Clk );
uint32_t ADC0Read( uint8_t channelNum );
uint32_t ADC1Read( uint8_t channelNum );
//extern void ADC0Handler( void ) __irq;
//extern void ADC1Handler( void ) __irq;
//extern uint32_t ADCInit( uint32_t ADC_Clk );
//extern uint32_t ADC0Read( uint8_t channelNum );
//extern uint32_t ADC1Read( uint8_t channelNum );

//extern volatile uint32_t ADC0Value[ADC_NUM], ADC1Value[ADC_NUM];
//extern volatile uint32_t ADC0IntDone, ADC1IntDone;

#endif /* end __ADC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
