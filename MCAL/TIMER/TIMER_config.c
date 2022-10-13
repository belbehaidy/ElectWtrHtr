/*
 * TIMER_config.c
 *
 *  Created on: Aug 26, 2022
 *      Author: basse
 */

#include "..\..\SHARED\ATMEGA32_Registers.h"
#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"
#include "..\..\SHARED\BIT_MATH.h"

#include "TIMER_config.h"
#include "TIMER_priv.h"

u8 Timers_u8MaxNum = TIMERS_MAX_NUM , Timer0Max = TIMER0_MAX , Timer2Max = TIMER2_MAX ;

Timer_t	Timers[TIMERS_MAX_NUM] = 	{
										{	TIMER0	, TC0_CS	, TC0_COM	, TC0_WGM 	, TC0_INT	},
										{	TIMER2	, TC2_CS	, TC0_COM	, TC0_WGM 	, TC2_INT	}
									};
