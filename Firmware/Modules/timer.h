/*
 * timer.h
 *
 *  Created on: 18/03/2013
 *      Author: Daniel
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "LPC214x.h"
#include "cpu_init.h"

void Timer_Init(void);

void Timer_RegisterCallback(void (*ptrfunc)(void));

void Timer_Enable();
#endif /* TIMER_H_ */
