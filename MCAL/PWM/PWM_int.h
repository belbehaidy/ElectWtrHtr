/*
 * PWM_int.h
 *
 *  Created on: Sep 5, 2022
 *      Author: basse
 */

#ifndef MCAL_PWM_PWM_INT_H_
#define MCAL_PWM_PWM_INT_H_

#define CPU_CLOCK					16000000UL
#define EXT_CLOCK					0xFFFFFFFF


/****************************************/
/*			Timers/Counters				*/
/****************************************/
#define TIMER1A				201			/*	Counter1A		*/
#define TIMER1B				202			/*	Counter1B		*/

/****************************************/
/* 		Clock selection Options			*/
/****************************************/
#define PWM_NO_CLK				10
#define PWM_PRES_1				11
#define PWM_PRES_8				12
#define PWM_PRES_32				13			//	Timer2 ONLY
#define PWM_PRES_64				14
#define PWM_PRES_128			15			//	Timer2 ONLY
#define PWM_PRES_256			16
#define PWM_PRES_1024			17
#define PWM_EXT_CLK_FALL		18			//	Timer0 & Timer1 ONLY
#define PWM_EXT_CLK_RISE		19			//	Timer0 & Timer1 ONLY

/****************************************/
/*		Compare Output Mode Options		*/
/****************************************/
#define COMP_NORMAL			20
#define COMP_TOG_ON_MATCH	21
#define COMP_NON_INVERTED	22
#define COMP_INVERTED		23

/********************************************************************************************/
/*							 OCR DDR PINS ACCESS INFORMATION								*/
/********************************************************************************************/
#define OC1B_DDR			*((volatile u8*)0x31)		/*	OC1B DDR register Address		*/
#define OC1B_PIN			4							/*	OC1B bit number on DDR Register	*/

#define OC1A_DDR			*((volatile u8*)0x31)		/*	OC1A DDR register Address		*/
#define OC1A_PIN			5							/*	OC1A bit number on DDR Register	*/

/****************************************/
/*	 Waveform Generation Mode Options	*/
/****************************************/
#define WGM_MODE_00			100		/* TIMERS ONLY	*/
#define WGM_PC_8_bit		101
#define WGM_PC_9_bit		102
#define WGM_PC_10_bit		103
#define WGM_MODE_04			104		/* TIMERS ONLY	*/
#define WGM_FAST_8_bit		105
#define WGM_FAST_9_bit		106
#define WGM_FAST_10_bit		107
#define WGM_PFC_ICR1		108
#define WGM_PFC_OCR1A		109
#define WGM_PC_ICR1			110
#define WGM_PC_OCR1A		111
#define WGM_MODE_12			112		/* TIMERS ONLY	*/
#define WGM_MODE_13			113		/*	Reserved	*/
#define WGM_FAST_ICR1		114
#define WGM_FAST_OCR1A		115

/****************************************/
/*		 Timer Interrupt Modes		*/
/****************************************/
#define TC_POLLING			40
#define TC_OVERFLOW_INT		41
#define TC_OUT_COMP_INT		42
#define TC1_A_OUT_COMP_INT	43
#define TC1_B_OUT_COMP_INT	44
#define TC1_INPUT_CAPT_INT	45

/********************************************************************************************/
/*								 Timer Interrupt Names										*/
/********************************************************************************************/
#define TOIE1				62			/* Timer/Counter1 Overflow Interrupt				*/
#define OCIE1B				63			/* Timer/Counter1 Output Compare Match B Interrupt	*/
#define OCIE1A				64			/* Timer/Counter1 Output Compare Match A Interrupt	*/
#define TICIE1				65			/* Timer/Counter1 I/P Capture Interrupt				*/



ES_t PWM_enuInit( void );

ES_t PWM_enuGetClock( u8 Copy_u8TimerNum , u32 *Copy_pu32TimerClk);

ES_t PWM_enuSetClkPrescaler( u8 Copy_u8TimerNum , u8 Copy_u8PrescalerValue );

ES_t PWM_enuGetClkSelect( u8 Copy_u8TimerNum , u16 *Copy_pu8TimerClkSelect );

ES_t PWM_enuSetCOM_Mode( u8 Copy_u8TimerNum , u8 Copy_u8COM_Mode );

ES_t PWM_enuGetCOM_Mode( u8 Copy_u8TimerNum , u8 *Copy_pu8TimerCOM_Mode);

ES_t PWM_enuSetWGM_Mode( u8 Copy_u8TimerNum , u8 Copy_u8WGM_Mode );

ES_t PWM_enuGetWGM_Mode( u8 Copy_u8TimerNum , u8 *Copy_pu8TimerWGM_Mode);

ES_t PWM_enuSetInterruptMode( u8 Copy_u8TimerNum , u8 Copy_u8TimerInterruptMode );////////////////////////////////////

ES_t PWM_enuGetInterruptMode( u8 Copy_u8TimerNum , u8 *Copy_pu8TimerInterruptMode );//////////////////////////////////////

ES_t PWM_enuSetDutyCycle( u8 Copy_u8TimerNum , f32 Copy_f32DutyCycle );////////////////////////////////////////////////////////

//ES_t PWM_enuReset( u8 Copy_u8TimerNum );

//ES_t Timer_enuReadCounterValue( u8 Copy_u8TimerNum , void *Copy_pCounterValue );

ES_t PWM_enuSetOCRnValue( u8 Copy_u8TimerNum , u16 Copy_u16OCRnValue );

ES_t PWM_enuReadOCRnValue( u8 Copy_u8TimerNum , void *Copy_pCounterValue );

ES_t PWM_enuSetICR1Value( u16 Copy_u16ICR1Value );

//ES_t Timer_enuReadICR1Value( u16 *Copy_pu16ICR1Value );

//ES_t PWM_enuInterruptEnable( u8 Copy_u8TimerIntName);

//ES_t PWM_enuInterruptDisable( u8 Copy_u8TimerIntName);

//bool PWM_IsInterruptEnabled( u8 Copy_u8TimerIntName );

ES_t PWM_enuCallBack( u8 Copy_u8TimerIntName , void (*Copy_pAppFun)(void) );

#endif /* MCAL_PWM_PWM_INT_H_ */
