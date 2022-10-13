/*
 * PWM_priv.h
 *
 *  Created on: Sep 5, 2022
 *      Author: basse
 */

#ifndef MCAL_PWM_PWM_PRIV_H_
#define MCAL_PWM_PWM_PRIV_H_

#include "..\..\SHARED\ATMEGA32_Registers.h"
#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"
#include "..\..\SHARED\BIT_MATH.h"

#define PWM		1

typedef struct
{
	u8 InterruptName ;
	void (*ptrFun)(void);
}PWM_Int_Pointers_t;



#define PWM_MAX_NUM				2
#define PWM_INTERRUPTS			3
#define CPU_CLOCK					16000000UL
#define EXT_CLOCK					0xFFFFFFFF

#define WGM_MODE_01			0x01
#define WGM_MODE_03			0x03
#define EIGHT_BIT_TOP		0x00FF
#define NINE_BIT_TOP		0x01FF
#define TEN_BIT_TOP			0x03FF
#define PC_NON_INV_OCR_VALUE( DutyCycle , TOP )		(u16)( (DutyCycle * TOP ) / 100UL )
#define PC_INV_OCR_VALUE( DutyCycle , TOP )			(u16)( TOP - ( ( DutyCycle * TOP ) / 100UL ) )
#define FAST_NON_INV_OCR_VALUE( DutyCycle , TOP )	(u16)( ( ( DutyCycle * ( TOP + 1UL ) ) / 100UL ) - 1.0 )
#define FAST_INV_OCR_VALUE( DutyCycle , TOP )		(u16)( TOP - ( ( DutyCycle * ( TOP + 1UL ) ) / 100UL ) )
#define PC_OCR_CALCULATOR	do{\
								if( Local_u8TimerCOM_Mode == COMP_NON_INVERTED )\
									Local_u16OCRValue = PC_NON_INV_OCR_VALUE( Copy_f32DutyCycle , Local_u16TimerTop );\
								else if( Local_u8TimerCOM_Mode == COMP_INVERTED )\
									Local_u16OCRValue = PC_INV_OCR_VALUE( Copy_f32DutyCycle , Local_u16TimerTop );\
								}while(0)
#define FAST_OCR_CALCULATOR	do{\
								if( Local_u8TimerCOM_Mode == COMP_NON_INVERTED && Copy_f32DutyCycle == 0.0 )\
									Local_u8Flag =1 ;\
								else if( Local_u8TimerCOM_Mode == COMP_INVERTED && Copy_f32DutyCycle == 100.0 )\
									Local_u8Flag =1 ;\
								else if( Local_u8TimerCOM_Mode == COMP_NON_INVERTED )\
									Local_u16OCRValue = FAST_NON_INV_OCR_VALUE( Copy_f32DutyCycle , Local_u16TimerTop );\
								else if( Local_u8TimerCOM_Mode == COMP_INVERTED )\
									Local_u16OCRValue = FAST_INV_OCR_VALUE( Copy_f32DutyCycle , Local_u16TimerTop );\
								}while(0)


#define BIT0_MASK			_BIT_MASK_
#define BIT1_MASK			2
#define TWO_BITS_MASK		_TWO_BITS_MASK_


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
#define PWM_PRES_64				14
#define PWM_PRES_256			16
#define PWM_PRES_1024			17
#define PWM_EXT_CLK_FALL		18			//	Timer0 & Timer1 ONLY
#define PWM_EXT_CLK_RISE		19			//	Timer0 & Timer1 ONLY

/****************************************/
/*		Compare Output Mode Options		*/
/****************************************/
#define COMP_NORMAL			20		/* TIMERS ONLY	*/
#define COMP_NON_INVERTED	22
#define COMP_INVERTED		23

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
/*		 Timer Interrupt Options		*/
/****************************************/
#define PWM_POLLING			50
#define PWM_OVERFLOW_INT	51
#define PWM_OUT_COMP_INT	52

/********************************************************************************************/
/*								 Timer Interrupt Names										*/
/********************************************************************************************/
#define TOIE1				62			/* Timer/Counter1 Overflow Interrupt				*/
#define OCIE1B				63			/* Timer/Counter1 Output Compare Match B Interrupt	*/
#define OCIE1A				64			/* Timer/Counter1 Output Compare Match A Interrupt	*/

/************************************************************************************************************/
/* 										TIMER/Counter1 Registers											*/
/************************************************************************************************************/
#define WGM10				0							/*	Timer/Counter1 Waveform Generation Mode (WGM10)	*/
#define WGM11				1							/*	Timer/Counter1 Waveform Generation Mode (WGM11)	*/
#define FOC1B_BIT			2							/*	Timer/Counter1 Force Output Compare	B			*/
#define FOC1A_BIT			3							/*	Timer/Counter1 Force Output Compare A			*/
#define COM1B0				4							/*******Timer/Counter1 B Compare Output Mode Bits****/
#define COM1B1				5							/***********COM1B1:COM1B0			Bits 5:4*********/
#define COM1A0				6							/*******Timer/Counter1 A Compare Output Mode Bits****/
#define COM1A1				7							/***********COM1A1:COM1A0			Bits 7:6*********/

#define WGM1A_SEL_BITS_MASK					0x03
#define WGM1A_SEL_BITS						0

#define COM1B_SEL_BITS_MASK					0x30
#define COM1B_SEL_BITS						4

#define COM1A_SEL_BITS_MASK					0xC0
#define COM1A_SEL_BITS						6
					/***********************************************************/

#define CS10				0							/*******Timer/Counter1 Clock Select Bits*************/
#define CS11				1							/*				CS12:CS11:CS10						*/
#define CS12				2							/**********	 Bits  2:  1 :  0		*****************/
#define WGM12_BIT			3							/*	Timer/Counter1 Waveform Generation Mode (WGM12)	*/
#define WGM13_BIT			4							/*	Timer/Counter1 Waveform Generation Mode (WGM13) */
/*	NOT USED	NOT USED	NOT USED	NOT USED	NOT USED	NOT USED	NOT USED	NOT USED	NOT USED*/
#define ICES1				6							/* 		Timer/Counter1 I/P Capture Edge Select	 	*/		//1 for Positive/Rising Edge & 0 for Negative/Falling Edge
#define ICNC1				7							/* Timer/Counter1 I/P Capture Noise Canceler Enable */

#define CLK1_SEL_BITS_MASK					0x07
#define CLK1_SEL_BITS						0
#define WGM1B_SEL_BITS_MASK					0x18
#define WGM1B_SEL_BITS						3

					/***********************************************************/



/********************************************************************************************************************/
/*										TIMER/Counter Interrupt Mask  Register										*/
/********************************************************************************************************************/
#define TOIE1_BIT							2			/* Timer/Counter1 Overflow Interrupt Enable bit				*/
#define OCIE1B_BIT							3			/* Timer/Counter1B Output Compare Match Interrupt Enable bit*/
#define OCIE1A_BIT							4			/* Timer/Counter1A Output Compare Match Interrupt Enable bit*/
#define TICIE1_BIT							5			/* Timer/Counter1 I/P Capture Interrupt Enable bit			*/
//#define TC0_INT_EN_MASK						0x03
#define TC1_INT_EN_MASK						0x3C
//#define TC2_INT_EN_MASK						0xC0

/****************************************************************************************************/
/*								TIMER/Counter Interrupt Flag  Register								*/
/****************************************************************************************************/
#define TOV1_BIT							2			/* Timer/Counter1 Overflow Flag bit			*/
#define OCF1B_BIT							3			/* Timer/Counter1 Output Compare 1B Flag bit*/
#define OCF1A_BIT							4			/* Timer/Counter1 Output Compare 1A Flag bit*/
#define ICF1_BIT							5			/* Timer/Counter1 I/P Capture Flag bit		*/
/****************************************************************************************************/
/*								Special Function IO  Register										*/
/****************************************************************************************************/
#define PSR10_BIT							0			/* Prescaler RESET Bit for Timer/Counter0&1	*/
//#define PSR2_BIT							1			/* Prescaler RESET Bit for Timer/Counter2	*/


#endif /* MCAL_PWM_PWM_PRIV_H_ */
