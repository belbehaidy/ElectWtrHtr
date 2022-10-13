/*
 * TIMER_config.h
 *
 *  Created on: Aug 26, 2022
 *      Author: basse
 */

#ifndef MCAL_TIMER_TIMER_CONFIG_H_
#define MCAL_TIMER_TIMER_CONFIG_H_

#include "TIMER_priv.h"

#define TIMERS_MAX_NUM				2

typedef struct
{
	u8 TimerNum;
	u8 ClkSelect;
	u8 CompOutMode;
	u8 WaveGenMode;
	u8 InterruptMode;
}Timer_t;

/************************************/
/*	Timer/Counter0	Configuration	*/
/************************************/
#define TC0_CS			PRES_1024
#define TC0_COM			COMP_NORMAL
#define	TC0_WGM			WGM_CTC_MODE
#define TC0_INT			TC_POLLING
#define TIMER0_BOTTOM	0x00
#define TIMER0_MAX		0xFF

/************************************/
/*		Timer2	Configuration		*/
/************************************/
#define TC2_CS			PRES_1024
#define TC2_COM			COMP_NORMAL
#define	TC2_WGM			WGM_CTC_MODE
#define TC2_INT			TC_POLLING
#define TIMER2_BOTTOM	0x00
#define TIMER2_MAX		0xFF


#endif /* MCAL_TIMER_TIMER_CONFIG_H_ */
