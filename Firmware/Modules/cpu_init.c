/* =============================== C/C++ SOURCE FILE ================================ */
/**
	\file
	\description
	\copyright (c) STHS Tecnologia
*/
/* ================================================================================== */
/*
CHANGES:
---------------
Date            Author       Description
01/02/2010      gilsonf      v1.0 Released
23/03/2011      fabiovince   v1.1
07/04/2011      fabiovince   v1.2
*/

/*------------------------------------------------------------------------------------*/
/*                                      INCLUDES                                      */
/*------------------------------------------------------------------------------------*/
#include "LPC214x.h"
#include "cpu_init.h"

/*------------------------------------------------------------------------------------*/
/*                               DEFINITIONS AND MACROS                               */
/*------------------------------------------------------------------------------------*/

#define PLOCK 0x400  // Frequency Lock (configurando o PLL
#define MEMMAP_BOOT_LOADER_MODE   0       // Interrupt vectors are re-mapped to Boot Block.
#define MEMMAP_USER_FLASH_MODE    (1<<0)  // Interrupt vectors are not re-mapped and reside in Flash.
#define MEMMAP_USER_RAM_MODE      (1<<1)  // Interrupt vectors are re-mapped to Static RAM.


/*------------------------------------------------------------------------------------*/
/*                          TYPEDEFS, CLASSES AND STRUCTURES                          */
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                                     PROTOTYPES                                     */
/*------------------------------------------------------------------------------------*/

void feed(void); // Utilizado para alimentar o PLL

/*------------------------------------------------------------------------------------*/
/*                                  GLOBAL VARIABLES                                  */
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                                  LOCAL  VARIABLES                                  */
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                              FUNCTION  IMPLEMENTATION                              */
/*------------------------------------------------------------------------------------*/

/***************************************************************************/
/*  Função que inicializa o microcontrolador                               */
/***************************************************************************/
void cpu_init(void)  {

    PLLCFG= cpuMHz;  // Configura o clock da CPU do ARM

    feed();

    // Enabling the PLL */
    PLLCON=0x1;
    feed();

    // Wait for the PLL to lock to set frequency
    while(!(PLLSTAT & PLOCK)) ;

    // Connect the PLL as the clock source
    PLLCON=0x3;
    feed();

    // Enabling MAM and setting number of clocks used for Flash memory fetch (4 cclks in this case)
    MAMCR=0x2;
    MAMTIM=0x4;

    // Configura o clock dos periféricos (pclk) de acordo com o clock do processador (cclk)
    VPBDIV=bus_freq; // Configura a frequência do barramento dos periféricos

    // maps interrupt vector to flash
    MEMMAP = MEMMAP_USER_FLASH_MODE;

    //  // provides extra safer initializations  (should not be necessary)
    //  VICIntSelect  =  0;             // Set all VIC interrupts to IRQ for now
    //  VICIntEnClr = 0xFFFFFFFF;     // Disable all interrupts
    //  VICSoftIntClr = 0xFFFFFFFF;   // Clear all software interrupts
    //  VICProtection = 0;              // VIC registers can be accessed in User or
    //									  // privileged mode
    //  VICVectAddr = 0;                // Clear interrupt
    //  VICDefVectAddr = 0;             // Clear address of the default ISR

}


/***************************************************************************/
/*  Função para comando
 * o PLL a executar o último comando.
 * O microcontrolador implementa isso como forma de segurança extra
 * no caso de inadvertidamente o código alterar a configuração do PLL      */
/***************************************************************************/

void feed(void)
{
	PLLFEED=0xAA; // olhar no datasheet
	PLLFEED=0x55;
}


/*------------------------------------------------------------------------------------*/
/*                                        EOF                                         */
/*------------------------------------------------------------------------------------*/
