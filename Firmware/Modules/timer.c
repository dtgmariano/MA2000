/*------------------------------------------------------------------------------------*/
/*                                       HEADER                                       */
/*------------------------------------------------------------------------------------*/

/***************************************************************************************
 *Project Name: MA2000 Firmware
 *	   Version: 0.24
 *   File Name: timer.c
 *  Created on: 19/03/2013
 *      Author: Daniel Teodoro G. Mariano
 *       Email: dtgm@uol.com.br
 * Description: Timer module configuration for the LPC2148 based on the
 * 				MA2000 project
***************************************************************************************/


/*------------------------------------------------------------------------------------*/
/*                                      INCLUDES                                      */
/*------------------------------------------------------------------------------------*/
#include "Modules/timer.h"
#include "stdio.h"

/*------------------------------------------------------------------------------------*/
/*                               DEFINITIONS AND MACROS                               */
/*------------------------------------------------------------------------------------*/
#define PRESCALEREGISTER 30
// Tempo = (PCLK/T0PR) * T0MR0, Ex: Tempo = (60 MHz/30) * 1000000 / 2 (Dois ciclos) = 1s
//#define MATCHREGISTER0 4
#define MATCHREGISTER0 8
#define MATCHREGISTER1 4


/*------------------------------------------------------------------------------------*/
/*                                     PROTOTYPES                                     */
/*------------------------------------------------------------------------------------*/
void T0isr(void) __attribute__ ((interrupt("IRQ")));//atribui a funcao a interrução IRQ
void (*mptrfunc)(void);


/*------------------------------------------------------------------------------------*/
/*                              FUNCTION  IMPLEMENTATION                              */
/*------------------------------------------------------------------------------------*/

/***************************************************************************************
 * Function Name: T0isr(void)
 * 		 	Type: Void
 *	  Parameters: None
 * 		 Returns: None
 * 	 Description:
***************************************************************************************/
void T0isr(void)
{
	if(mptrfunc != NULL)
	{
		mptrfunc();
	}

	T0IR 		|= 0x00000001;			//Clear match 0 interrupt
	VICVectAddr  = 0x00000000;			//Dummy write to signal end of interrupt
}

/***************************************************************************************
 * Function Name: Timer_Init(void)
 * 		 	Type: Void
 *	  Parameters: None
 * 		 Returns: None
 * 	 Description: Initiate Timer Module
***************************************************************************************/
void Timer_Init(void)
{
	T0PR = PRESCALEREGISTER; //incrementa o contador, quando o contador prescale atinge o valor de PR.

	T0TCR = 0x00000002;	    //desabilita o contador e reseta o
	T0MCR = 0x00000003;	    //On match reset the counter and generate an interrupt
	T0MR0 = MATCHREGISTER0;  // T0MR0 = Match Register 0,
						    // Tempo = (CLOCK dos periféricos (PCLK) / Prescaler (T0PR)) *  Match Register (T0MR0)

	//T0MR1 = 0x00000000;		// Set duty cycle to zero
	T0MR1 = MATCHREGISTER1;
//	T0TCR = 0x00000001;		//enable timer

	VICVectAddr4 = (unsigned)T0isr;		//Set the timer ISR vector address
	VICVectCntl4 = 0x00000024;			//Set channel
	VICIntSelect &= ~(1<<4);			//seleciona a se a interrupção sera FIQ ou IRQ para o timmer0
	//VICIntEnable |= 0x00000010;			//habilita a interrupção de timer 0
}

/***************************************************************************************
 * Function Name: Timer_RegisterCallback(void)
 * 		 	Type: Void
 *	  Parameters: A pointer of a function
 * 		 Returns: Void
 * 	 Description: Initiate Timer Module
***************************************************************************************/
void Timer_RegisterCallback(void (*ptrfunc)(void))
{
	//VICVectaddr4 = ptrfunc;
	mptrfunc = ptrfunc;
}

/***************************************************************************************
 * Function Name: Timer_Enable(void)
 * 		 	Type: Void
 *	  Parameters: None
 * 		 Returns: None
 * 	 Description: Initiate Timer Module
***************************************************************************************/
void Timer_Enable()
{
	VICIntEnable |= 0x00000010;
	T0TCR = 0x00000001;
}
