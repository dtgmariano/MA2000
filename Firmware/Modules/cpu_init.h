/* =============================== C/C++ HEADER FILE ================================ */
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

#ifndef cpu_init_H_
#define cpu_init_H_

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

/*------------------------------------------------------------------------------------*/
/*                                      INCLUDES                                      */
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                               DEFINITIONS AND MACROS                               */
/*------------------------------------------------------------------------------------*/

// De acordo com o UM10139 Volume 1: LPC214x User Manual, a configuração da freqüência do CLOCK é
// fornecida por ( CCLK = M × FOSC).
//
//Onde:
//	CCLK: valor da freqüência do CLOCK do processador;
//	M: valor do multiplicador do PLL que controla a da freqüência do CLOCK do processador;
//	Fosc: frequência do oscilador a cristal/oscilador externo.
//
//	Deste modo, supondo que Fosc = 12MHz e CCLK = 60 MHz:
//	M = CCLK / Fosc = 60 MHz / 12 MHz = 5. Consequentemente M - 1 = 4 deverá ser escrito no registrador
// 	PLLCFG[4:0] que possui a função de configurar o valor do multiplicador e divisor do PLL.
//	O valor do divisor ( P ) do PLL dever ser configurado de tal forma que a frequência do PLL (FCCO)
//  fique entre 156 MHz e 320 MHz. Deste modo, para Fcco = 156 MHz, P = 156 MHz / (2 x 60 MHz) = 1.3
//  e para Fcco = 320 MHz, P = 320 MHz / (2 x 60 MHz) = 2.67. Portanto, o único valor inteiro para P
// 	que satisfaz ambas as condições de acordo com a Tabela 22 do UM10139 Volume 1: LPC214x User Manual
// 	é P = 2. Deste modo, de acordo com a Tabela 22, o valor 1 deverá ser escrito em PLLCFG[6:5].


// Considerando um cristal de 12 MHz, o valor do Mutiplicador e Divisor são:
#define cristal_12MHz_cpu_60MHz 0x24  // 0x24 - Clock do processador (cclk) configurado para 60MHz
#define cristal_12MHz_cpu_48MHz 0x23  // 0x23 - Clock do processador (cclk) configurado para 48MHz
#define cristal_12MHz_cpu_36MHz 0x42  // 0x42 - Clock do processador (cclk) configurado para 36MHz
#define cristal_12MHz_cpu_24MHz 0x41  // 0x41 - Clock do processador (cclk) configurado para 24MHz
#define cristal_12MHz_cpu_12MHz 0x60  // 0x60 - Clock do processador (cclk) configurado para 12MHz

//--------------------------------------------------------------------------------------
//--- A frequência da CPU deve ser configurada aqui!!! ---------------------------------
#define cpuMHz cristal_12MHz_cpu_60MHz   // Define o clock da CPU do ARM


#define bus_div0 0x01  // Barramento com a mesma frequência da CPU
#define bus_div2 0x10  // Barramento com metade (1/2) da frequência da CPU
#define bus_div4 0x00  // Barramento com um quarto (1/4) da frequência da CPU


//--------------------------------------------------------------------------------------
//--- A frequência do barramento dos periféricos deve ser configurada aqui!!! ----------
#define bus_freq bus_div0   // Define a frequência do barramento dos periféricos



/*------------------------------------------------------------------------------------*/
/*                          TYPEDEFS, CLASSES AND STRUCTURES                          */
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                                  GLOBAL VARIABLES                                  */
/*------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------*/
/*                                  GLOBAL FUNCTIONS                                  */
/*------------------------------------------------------------------------------------*/

	void cpu_init(void);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* cpu_init_H_ */

/*------------------------------------------------------------------------------------*/
/*                                          EOF                                       */
/*------------------------------------------------------------------------------------*/



