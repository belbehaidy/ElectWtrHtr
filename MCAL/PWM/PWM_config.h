/*
 * PWM_config.h
 *
 *  Created on: Sep 5, 2022
 *      Author: basse
 */

#ifndef MCAL_PWM_PWM_CONFIG_H_
#define MCAL_PWM_PWM_CONFIG_H_

#include "PWM_priv.h"

typedef struct
{
	u8 TimerNum;
	u8 ClkSelect;
	u8 CompOutMode;
	u8 WaveGenMode;
	u8 InterruptMode;
}PWM_t;

/************************************/
/*	Timer/Counter1	Configuration	*/
/************************************/
#define TC1_CS			PWM_PRES_256
#define	TC1_WGM			WGM_FAST_ICR1
#define TIMER1_BOTTOM	0x0000
#define TIMER1_MAX		0x0270
/************************************/
/*	Timer/Counter1A	Configuration	*/
/************************************/
#define TC1A_COM		COMP_INVERTED
#define TC1A_INT		PWM_OUT_COMP_INT
/************************************/
/*	Timer/Counter1B	Configuration	*/
/************************************/
#define TC1B_COM		COMP_INVERTED
#define TC1B_INT		PWM_OUT_COMP_INT

#endif /* MCAL_PWM_PWM_CONFIG_H_ */
