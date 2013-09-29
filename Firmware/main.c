/*------------------------------------------------------------------------------------*/
/*                                       HEADER                                       */
/*------------------------------------------------------------------------------------*/
/***************************************************************************************
 *Project Name: MA2000 Firmware
 *	   Version: 0.24
 *   File Name: main.c
 *  Created on: 19/03/2013
 *      Author: Daniel Teodoro G. Mariano
 *       Email: dtgm@uol.com.br
 * Description: Firmware for LPC2148, to control the MA2000 robot, a mechanical arm
 * 				with 6 degrees of freedom.
 *
 * 				The first three DOF's (1-Waist, 2-Shoulder, 3-Elbow) are controlled each
 * 				one by a DC Motor with the following configurations: A PWM at 20000 Hz with
 * 				variable Duty Cycle is assigned to the variable DOF_PWM_DC, according with
 * 				the proportional control and two GPIO for controlling the inputs of the
 * 				LPC6203 Full Bridge Driver.
 *
 * 				Each motor has a potentiometer connected to the motor shaft by a toothed
 * 				belt, and this potentiometer provides a voltage signal proportional to the
 * 				angular position of the motor. This signal is capture by an ADC channel
 * 				and is assigned to the variable DOF_ADC_POT
 *
 * 				The proportional control is effected by the relationship of the variables
 * 				DOF_ADC_POT and the setpoint position established by the user assigned to
 * 				the variable DOF_SETPOINT.
 *
 * 				The others three DOF's (4-Pitch, 5-Yaw, 6-Roll) are controlled each one
 * 				by a Servomotor with the following configurations: A PWM at 50 Hz with
 * 				variable Duty Cycle (5%-10%) to control the servo.
***************************************************************************************/

/*------------------------------------------------------------------------------------*/
/*                                PINS CONFIGURATION                                  */
/*------------------------------------------------------------------------------------*/
/***************************************************************************************
* Pins		Function	  	LPC2148 	Connector
* P0.7		PWM 2 (31)      EXT1-8		EXT1 20
* P0.8		PWM 4 (31)      			EXT1 17
* P0.21		PWM 5 (31)           		EXT1 9
* P0.18		P0.18 (53)      EXT1-19		EXT1 7
* P0.19		P0.19 (54)      EXT1-20		EXT1 8
* P0.28		AD0.1 (13)      EXT2-1		EXT2 25
* P0.29		AD0.2 (14)      EXT2-2		EXT2 26
* P0.30		AD0.3 ()
* P0.25		AD0.4
*
* 				  			+3.3V   	EXT2-4
* 			      			GND     	EXT2-1 e EXT1-2
***************************************************************************************/

/*------------------------------------------------------------------------------------*/
/*                                 	   INCLUDES                                       */
/*------------------------------------------------------------------------------------*/
#include <string.h>
#include "LPC214x.h"
#include "Modules/adc.h"
#include "Modules/pwm.h"
#include "Modules/gpio.h"
#include "Modules/uart.h"
#include "Modules/iprintf.h"
#include "Modules/timer.h"

/*------------------------------------------------------------------------------------*/
/*                                 	   DEFINES                                        */
/*------------------------------------------------------------------------------------*/
#define PWMSERVO_COUNT 4290
#define PLOCK 0x400

/*------------------------------------------------------------------------------------*/
/*                                Variable Declaration                                */
/*------------------------------------------------------------------------------------*/

uint8_t buffer[100] = {0};					//Buffer Circular para dados da UART
int size = 0;								//Variável para medir o tamanho do buffer da UART
int data[9] = {0};							//Variável para armazenamento de dados da UART

int count = 0;								//Contador

uint32_t sp_svm[3] 	= {0};					//Set point para os GDL dos servomotores
uint32_t adc[3] 	= {0}; 					//Sensor potenciométrico de cada GDL
uint32_t sp[3] 		= {512,512,512};  		//Set point de cada GDL
uint32_t e[3] 		= {0};					//Erro de cada GDL (e = sp - adc)
uint32_t kp[3] 		= {0};					//Cte proporcional do PID
uint32_t ki[3] 		= {0};					//Cte integrativa do PID
uint32_t kd[3] 		= {0};					//Cte derivativa do PID
uint32_t saida[3]	= {0};					//Saída do PID
float PTerm[3]	= {0};

int PLimit = 0x7FFF;                    //Excursion Limit for Proportional component
int ILimit = 0x7FFF;                    //Excursion limit for Integrative component
int DLimit = 0x7FFF;                    //Excursion limit for Derivative component
int Out_Limit = 0x7FFF;                    //Excursion limit for Output

/*------------------------------------------------------------------------------------*/
/*                                     Initialization                                 */
/*------------------------------------------------------------------------------------*/

static void feed(void)
{
  PLLFEED = 0xAA;
  PLLFEED = 0x55;
}

int Limit_value_signed ( int Variable, int Min_Value, int Max_Value)
{
	if (Variable < Min_Value)
	{Variable = Min_Value;}
	if (Variable > Max_Value)
	{Variable = Max_Value;}
	return Variable;
}

int Adjust_PWM_to_unsigned(int Variable)
{
	if (Variable<0)
	{Variable = -Variable;}
	return  Variable;
}

void Initialize(void)
{

	PLLCFG = 0x24; //Configura o multiplicador PLL para 4 e o divisor para 2
             	   //Assim com o cristal de 12MHz temos um CCLK = 48MHz e um FCCO = 192MHz
    feed();

    PLLCON = 0x01; //Habilita o bloco PLL
    feed();      //Para que as alterações anteriores tenham efeito

    while(!(PLLSTAT & PLOCK)) ; //Garante que a alteração anterior teve efeito uma vez que quando
                    //o bit PLOCK do reg. PLLSTAT vale zero ouve um bloqueio no PLL que
                    //ocorre logo após a modificação dos registradores

    PLLCON = 0x03; //Depois de habilitado resta conectá-lo a fonte de clock para que comece o trabalho
    feed();        //Necessário para que as alterações anteriores tenham efeito

    MAMCR = 0x02;  //Habilita todas as funções do Módulo acelerador de memória
    MAMTIM = 0x04;  //O ciclo de busca do MAM tem duração de 4 ciclos
    VPBDIV = 0x01;  //Clock dos periféricos igual ao clock do Sistema

    ADC_Init(ADC_CLK);
    UART_Init(0,9600);
    PWM_Init();
    GPIO_Init();
    //	Timer_Init();
    //	Timer_RegisterCallback(PWM_InterruptControl);
    //	Timer_Enable();
}

/*------------------------------------------------------------------------------------*/
/*                               FUNCTIONS  IMPLEMENTATION                            */
/*------------------------------------------------------------------------------------*/

void delay(uint32_t x)
{
  for(int i = 0; i < x; i++)
  {
    asm volatile ("NOP");
    asm volatile ("NOP");
    asm volatile ("NOP");
    asm volatile ("NOP");
    asm volatile ("NOP");
    asm volatile ("NOP");
    asm volatile ("NOP");
  }
}

void ReceivingData(void)
{
	size = UART_GetNumberAvailableElements();
	if(size>=36)
	{
		UART_ReceiveBuffer(0,buffer,36);
	  	memcpy(data,buffer,sizeof(int)*9);
	  	sp[0] = data[0];	  	sp[1] = data[1];	  	sp[2] = data[2];
	  	kp[0] = data[3];	  	kp[1] = data[4];	  	kp[2] = data[5];
	  	ki[0] = data[6];	  	ki[1] = data[7];	  	ki[2] = data[8];
//	  	kd[0] = data[9];	  	kd[1] = data[10];	  	kd[2] = data[11];
	}
}

void ReadingADC(void)
{
	adc[0] = ADC0Read(3);	delay(10);
	//adc[1] = ADC0Read(1);	delay(10);
	//adc[2] = ADC0Read(2);	delay(10);
}

void PIDController(void)
{
	//**************DOF 1**************//
	if((sp[0]>adc[0]) || adc[0] < 50)
	{	IOCLR0 = (1<<23); 	IOSET0 = (1<<22);	}
	if((sp[0]<adc[0]) || adc[0] > 1000)
	{	IOCLR0 = (1<<22);	IOSET0 = (1<<23);	}

	e[0] = sp[0] - adc[0];

	//proportional term
	PTerm[0] = (kp[0] * e[0])/10.0;
	if (e[0] > 0)
	{    if (PTerm[0] < 0) { PTerm[0] = 0x7FFF;}    }
	if (e[0] < 0)
	{    if (PTerm[0] > 0) { PTerm[0] = 0x8000;}    }
	PTerm[0] = Limit_value_signed(PTerm[0], -1023 , 1023);

	saida[0] = PTerm[0];
	saida[0] = Limit_value_signed(saida[0],-1023, 1023);
	saida[0] = Adjust_PWM_to_unsigned(saida[0]);

	setDutyCycle(5,saida[0]);  //DOF 1

	//**************DOF 2**************//
	if((sp[1]>adc[1]) || adc[1] < 50)
	{	IOCLR0 = (1<<17);	IOSET0 = (1<<19);	}
	if((sp[1]<adc[1]) || adc[1] > 1000)
	{	IOCLR0 = (1<<19);	IOSET0 = (1<<17);	}

	e[1] = sp[1] - adc[1];

	//proportional term
	PTerm[1] = (kp[1] * e[1])/10.0;
	if (e[1] > 0)
	{    if (PTerm[1] < 0) { PTerm[1] = 0x7FFF;}    }
	if (e[1] < 0)
	{    if (PTerm[1] > 0) { PTerm[1] = 0x8000;}    }
	PTerm[1] = Limit_value_signed(PTerm[1], -PLimit , PLimit);

	saida[1] = PTerm[1];
	saida[1] = Limit_value_signed(saida[1],-1023,1023);
	saida[1] = Adjust_PWM_to_unsigned(saida[1]);

	setDutyCycle(5,saida[1]);  //DOF 1

	//**************DOF 3**************//
	if(sp[2]>adc[2] || adc[2] < 50)
	{	IOCLR0 = (1<<6); 	IOSET0 = (1<<9);	}
	if((sp[2]<adc[2]) || adc[2] > 1000)
	{	IOCLR0 = (1<<9);	IOSET0 = (1<<6);	}

	e[2] = sp[2] - adc[2];

	//proportional term
	PTerm[2] = (kp[2] * e[2])/10.0;
	if (e[0] > 0)
	{    if (PTerm[2] < 0) { PTerm[2] = 0x7FFF;}    }
	if (e[2] < 0)
	{    if (PTerm[2] > 0) { PTerm[2] = 0x8000;}    }
	PTerm[2] = Limit_value_signed(PTerm[2], -PLimit , PLimit);

	saida[2] = PTerm[2];
	saida[2] = Limit_value_signed(saida[2],-1023,1023);
	saida[2] = Adjust_PWM_to_unsigned(saida[2]);

	setDutyCycle(2,saida[2]);  //DOF 1
}

void SendingData(void)
{
	if(count>=5000)
	{
		iprintf("1- %d  %d  \r\n", e[0], saida[0]);
		iprintf("2- %d  %d  \r\n", e[1], saida[1]);
		iprintf("3- %d  %d  \r\n", e[2], saida[2]);
		count=0;
	}
	count++;
}


/***************************************************************************************
 * Function Name: main(void)
 * 		 	Type: Integer
 *	  Parameters:
 * 		 Returns: 0
 * 	 Description: Main function for firmware
***************************************************************************************/
int main (void)
{
	Initialize();
	while(1)
	{
		IOCLR0 = (1<<9);	IOSET0 = (1<<6);
		setDutyCycle(2,1000);  //DOF 1
//		ReceivingData();
//		ReadingADC();
//		PIDController();
//		SendingData();
	}
	return(0);
}
