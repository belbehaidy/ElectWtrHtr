/*
 * TIMER_priv.h
 *
 *  Created on: Aug 26, 2022
 *      Author: basse
 */

#ifndef MCAL_TIMER_TIMER_PRIV_H_
#define MCAL_TIMER_TIMER_PRIV_H_

#define Mask	0

typedef struct
{
	u8 InterruptName;
	void (*ptrFun)(void);
}TimerInt_Pointers_t;


#define TIMERS_INTERRUPTS			4
#define CPU_CLOCK					16000000UL
#define EXT_CLOCK					0xFFFFFFFF

#define WGM_MODE_00			0x00
#define WGM_MODE_04			0x01
#define WGM_MODE_12			0x03

#define BIT0_MASK			_BIT_MASK_
#define BIT1_MASK			0x02
#define TWO_BITS_MASK		_TWO_BITS_MASK_


/****************************************/
/*			Timers/Counters				*/
/****************************************/
#define	TIMER0				200			/*	Timer/Counter0	*/
#define	TIMER2				203			/*		Timer2		*/
/****************************************/
/* 		Clock selection Options			*/
/****************************************/
#define NO_CLK				10
#define PRES_1				11
#define PRES_8				12
#define PRES_32				13			//	Timer2 ONLY
#define PRES_64				14
#define PRES_128			15			//	Timer2 ONLY
#define PRES_256			16
#define PRES_1024			17
#define EXT_CLK_FALL		18			//	Timer0 & Timer1 ONLY
#define EXT_CLK_RISE		19			//	Timer0 & Timer1 ONLY

/****************************************/
/*		Compare Output Mode Options		*/
/****************************************/
#define COMP_NORMAL			20
#define COMP_TOG_ON_MATCH	21

/****************************************/
/*	 Waveform Generation Mode Options	*/
/****************************************/
#define WGM_NORMAL_MODE		30
#define WGM_CTC_MODE		32

/****************************************/
/*		 Timer Interrupt Options		*/
/****************************************/
#define TC_POLLING			40
#define TC_OVERFLOW_INT		41
#define TC_OUT_COMP_INT		42
/********************************************************************************************/
/*								 Timer Interrupt Names										*/
/********************************************************************************************/
#define TOIE0				60			/* Timer/Counter0 Overflow Interrupt				*/
#define OCIE0				61			/* Timer/Counter0 Output Compare Match Interrupt	*/
#define TOIE2				66			/* Timer/Counter2 Overflow Interrupt				*/
#define OCIE2				67			/* Timer/Counter2 Output Compare Match Interrupt 	*/

/************************************************************************************************************/
/* 										TIMER/Counter0 Registers											*/
/************************************************************************************************************/
#define CS00				0							/*******Timer/Counter0 Clock Select Bits*************/
#define CS01				1							/*				CS02:CS01:CS00						*/
#define CS02				2							/**********	 Bits  2:  1 :  0		*****************/
#define WGM01_BIT			3							/*	Timer/Counter0 Waveform Generation Mode (CTC)	*/
#define COM00				4							/********Timer/Counter0 Compare Output Mode Bits*****/
#define COM01				5							/********	COM01:COM00				Bits 5:4	*****/
#define WGM00_BIT			6							/*	Timer/Counter0 Waveform Generation Mode (PWM)	*/
#define FOC0_BIT			7							/*		Timer/Counter0 Force Output Compare			*/

#define CLK0_SEL_BITS_MASK					0x07
#define CLK0_SEL_BITS						0

#define COMP0_MAT_OUT_MODE_BITS_MASK		0x30
#define COMP0_MAT_OUT_MODE_BITS				4

/************************************************************************************************************/
/* 										TIMER/Counter2 Registers											*/
/************************************************************************************************************/
#define CS20								0			/*************Timer2 Clock Select Bits***************/
#define CS21								1			/*					CS22:CS21:CS20					*/
#define CS22								2			/************	 Bits  2:  1 :  0		*************/
#define WGM21_BIT							3			/*		Timer2 Waveform Generation Mode (CTC)		*/
#define COM20								4			/***********Timer2 Compare Output Mode Bits**********/
#define COM21								5			/***********COM21:COM20				Bits 5:4*********/
#define WGM20_BIT							6			/*		Timer2 Waveform Generation Mode (PWM)		*/
#define FOC2_BIT							7			/*		Timer2 Force Output Compare					*/

#define CLK2_SEL_BITS_MASK					0x07
#define CLK2_SEL_BITS						0

#define COMP2_MAT_OUT_MODE_BITS_MASK		0x30
#define COMP2_MAT_OUT_MODE_BITS				4


/************************************************************************************************************/
/* 									Asynchronous Status Register											*/
/************************************************************************************************************/
#define AS2									3			/*			Asynchronous Timer2	Enable Bit			*/
#define TCN2UB								2			/*				Timer2	Update Busy Flag			*/
#define OCR2UB								1			/*	Timer2 Output Compare Register Update Busy Flag	*/
#define TCR2UB								0			/*		Timer2 Control Register Update Busy Flag	*/

/********************************************************************************************************************/
/*										TIMER/Counter Interrupt Mask  Register										*/
/********************************************************************************************************************/
#define TOIE0_BIT							0			/* Timer/Counter0 Overflow Interrupt Enable bit				*/
#define OCIE0_BIT							1			/* Timer/Counter0 Output Compare Match Interrupt Enable bit	*/
#define TOIE2_BIT							6			/* Timer/Counter2 Overflow Interrupt Enable bit				*/
#define OCIE2_BIT							7			/* Timer/Counter2 Output Compare Match Interrupt Enable bit	*/

#define TC0_INT_EN_MASK						0x03
//#define TC1_INT_EN_MASK						0x3C
#define TC2_INT_EN_MASK						0xC0

/****************************************************************************************************/
/*								TIMER/Counter Interrupt Flag  Register								*/
/****************************************************************************************************/
#define TOV0_BIT							0			/* Timer/Counter0 Overflow Flag bit			*/
#define OCF0_BIT							1			/* Timer/Counter0 Output Compare Flag bit	*/
#define TOV2_BIT							6			/* Timer/Counter2 Overflow Flag bit			*/
#define OCF2_BIT							7			/* Timer/Counter2 Output Compare Flag bit	*/

/****************************************************************************************************/
/*								Special Function IO  Register										*/
/****************************************************************************************************/
#define PSR10_BIT							0			/* Prescaler RESET Bit for Timer/Counter0&1	*/
#define PSR2_BIT							1			/* Prescaler RESET Bit for Timer/Counter2	*/

#endif /* MCAL_TIMER_TIMER_PRIV_H_ */
