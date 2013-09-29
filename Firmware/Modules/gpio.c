/*------------------------------------------------------------------------------------*/
/*                                       HEADER                                       */
/*------------------------------------------------------------------------------------*/

/***************************************************************************************
 *Project Name: MA2000 Firmware
 *	   Version: 0.24
 *   File Name: gpio.c
 *  Created on: 19/03/2013
 *      Author: Daniel Teodoro G. Mariano
 *       Email: dtgm@uol.com.br
 * Description: GPIO Module with the purpose of setting the output pins
***************************************************************************************/

/*------------------------------------------------------------------------------------*/
/*                                 	   INCLUDES                                       */
/*------------------------------------------------------------------------------------*/
#include "LPC214x.h"
#include "gpio.h"

/*------------------------------------------------------------------------------------*/
/*                               FUNCTIONS  IMPLEMENTATION                            */
/*------------------------------------------------------------------------------------*/

/***************************************************************************************
 * Function Name: GPIO_Init()
 * 		 	Type: Void
 *	  Parameters: None
 * 	 	 Returns: Void
 * 	 Description: Sets P0.6, P0.9, P0.17, P0.19, P0.22, P0.23, P1.17, P1.19, P1.21 as GPIO
***************************************************************************************/

void GPIO_Init()
{
	//Setting GPIO Port 0.18 function: Sets P0.18 with value 00 for Bits 5 and 4 (PINSEL1)
	//P0.18       EXT1-19		EXT1 7
	//PINSEL1 &= ~((1<<4)|(1<<5));
	//IODIR0 |= (1<<18);	//Setting P0.18 as output

	//Setting GPIO Port 0.19 function: Sets P0.19 with value 00 for Bits 7 and 6 (PINSEL1)
	//P0.19      EXT1-20		EXT1 8
	//PINSEL1 &= ~((1<<6)|(1<<7));
	//IODIR0 |= (1<<19);	//Setting P0.19 as output

	PINSEL0 &= ~((1<<13)|(1<<12)); //Setting GPIO P0.6  Function ->
	PINSEL0 &= ~((1<<19)|(1<<18)); //Setting GPIO P0.9  Function -> Input 1B (DOF1)
	PINSEL1 &= ~((1<<3) |(1<<2) ); //Setting GPIO P0.17 Function ->
	PINSEL1 &= ~((1<<7) |(1<<6) ); //Setting GPIO P0.19 Function ->
	PINSEL1 &= ~((1<<13)|(1<<12)); //Setting GPIO P0.22 Function ->
	PINSEL1 &= ~((1<<15)|(1<<14)); //Setting GPIO P0.23 Function ->
//	PINSEL2 &= ~(1<<3);// Setting GPIO P1.25-16 Function -> Input 4, 5, 6 (DOF 4, 5, 6)

	IODIR0 |= (1<<6);  //Input 1A (DOF1)
	IODIR0 |= (1<<9);  //Input 1B (DOF1)
	IODIR0 |= (1<<17); //Input 2A (DOF2)
	IODIR0 |= (1<<19); //Input 2B (DOF2)
	IODIR0 |= (1<<22); //Input 3A (DOF3)
	IODIR0 |= (1<<23); //Input 3B (DOF3)
//	IODIR1 |= (1<<17); //Input 4 (DOF4)
//	IODIR1 |= (1<<19); //Input 6 (DOF6)
//	IODIR1 |= (1<<21); //Input 5 (DOF5)
}

