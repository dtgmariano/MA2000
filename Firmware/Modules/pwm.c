/*------------------------------------------------------------------------------------*/
/*                                       HEADER                                       */
/*------------------------------------------------------------------------------------*/

/***************************************************************************************
 *Project Name: MA2000 Firmware
 *	   Version: 0.24
 *   File Name: pwm.c
 *  Created on: 19/03/2013
 *      Author: Daniel Teodoro G. Mariano
 *       Email: dtgm@uol.com.br
 * Description: Sets PWM Module for LPC2148, based on the MA2000 Robot Project
***************************************************************************************/

#include "LPC214x.h"
#include "pwm.h"

#define PWMFREQUENCY 20000
#define CALCPWM (60000000/PWMFREQUENCY)

void PWM_Init()
{
	//Setting PWM2 function: Sets P0.7 with value 10 for Bits 15 and 14 (PINSEL0)
	//P0.7 -> EXT1-8 -> Pin 20
	PINSEL0 &= ~(3 << 14);
	PINSEL0 |= (1 << 15);

	//Setting PWM4 function: Sets P0.8 with value 10 for Bits 17 and 16 (PINSEL0)
	//P0.8 -> EXT1-9 -> Pin 17
	PINSEL0 &= ~(3 << 16);
	PINSEL0 |= (1 << 17);

	//Setting PWM5 function: Sets P0.21 with value 01 for Bits 11 and 10 (PINSEL1)
	//P0.21 -> EXT1-22 -> Pin 6
	PINSEL1 &= ~(3 << 11);
	PINSEL1 |= (1 << 10);

	//PWM Timer Control Register. The PWMTCR is used to control the Timer
	//Counter functions. The Timer Counter can be disabled or reset through
	//the PWMTCR.
	PWMTCR = 0x02; // reset PWM

	//PWMMR0 = 60000000/PWMFREQUENCY;
	PWMMR0 = CALCPWM;

	//PWM Prescale Register. The PWMTC is incremented every PWMPR+1
	//cycles of PCLK.
	PWMPR = 0; // Prescale unable

	//PWM Control Register. Enables PWM outputs and selects PWM channel
	//types as either single edge or double edge controlled.
	PWMPCR &= ~((1 << 2) | (1 << 4) | (1 << 5));
	PWMPCR |= ((1 << 13) | (1 << 12) | (1 << 10)); // Sets PWM 2,4,5 as single edge output
	//PWMPCR = 0x0000200; 	// ativa PWM1 como saída para PWM de borda simples

	//PWM Timer Control Register. The PWMTCR is used to control the Timer
	//Counter functions. The Timer Counter can be disabled or reset through
	//the PWMTCR.
	PWMTCR = 00001001; // Counter and PWM enabled

	//The PWM Match Control Register is used to control what operations are performed when
	//one of the PWM Match Registers matches the PWM Timer Counter.
	PWMMCR = 0x0000003;

//	setDutyCycle(2, 100);
//	setDutyCycle(4, 300);
//	setDutyCycle(5, 700);

	PWMMCR = 0x0000003;  // reset on Match 0
}

void setDutyCycle(unsigned int pwmchannel, unsigned int dutyccycle)
{
	     if(pwmchannel == 1){PWMMR1 = (dutyccycle  * CALCPWM)/1024;}
	else if(pwmchannel == 2){PWMMR2 = (dutyccycle  * CALCPWM)/1024;}
	else if(pwmchannel == 3){PWMMR3 = (dutyccycle  * CALCPWM)/1024;}
	else if(pwmchannel == 4){PWMMR4 = (dutyccycle  * CALCPWM)/1024;}
	else if(pwmchannel == 5){PWMMR5 = (dutyccycle  * CALCPWM)/1024;}
	else if(pwmchannel == 6){PWMMR6 = (dutyccycle  * CALCPWM)/1024;}
}

/*void setDutyCycle(unsigned int pwmchannel, unsigned int dutyccycle){
	if(pwmchannel == 1){PWMMR1 = (600000/PWMFREQUENCY) * dutyccycle;}
	else if(pwmchannel == 2){PWMMR2 = (600000/PWMFREQUENCY) * dutyccycle;}
	else if(pwmchannel == 3){PWMMR3 = (600000/PWMFREQUENCY) * dutyccycle;}
	else if(pwmchannel == 4){PWMMR4 = (600000/PWMFREQUENCY) * dutyccycle;}
	else if(pwmchannel == 5){PWMMR5 = (600000/PWMFREQUENCY) * dutyccycle;}
	else if(pwmchannel == 6){PWMMR6 = (600000/PWMFREQUENCY) * dutyccycle;}
}*/
