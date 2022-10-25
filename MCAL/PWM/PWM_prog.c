/*
 * PWM_prog.c
 *
 *  Created on: Sep 5, 2022
 *      Author: basse
 */

#include "..\..\SHARED\ATMEGA32_Registers.h"
#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"
#include "..\..\SHARED\BIT_MATH.h"

#include "PWM_config.h"
#include "PWM_priv.h"

static u32 PWM_u32TimerClock( u8 Copy_u8TimerNum ,u8 Copy_u8ClkSelectNum ,u8 *Copy_pu8ClkSelectPrescalar );

extern u8 PWM_u8MaxNum ;
extern u16 Timer1Max ;
extern PWM_t PWMs[];


static u16 Global_u16OCR1A_Value = 0x0000 , Global_u16OCR1B_Value = 0x0000 , Global_u16ICR1_Value = 0x0000;
static u32 Global_u32Timer1_Clk ;


PWM_Int_Pointers_t Global_AstrPWM_Pointers[PWM_INTERRUPTS] =	{
																	{ TOIE1 ,	NULL},
																	{ OCIE1B ,	NULL},
																	{ OCIE1A ,	NULL},
																};


ES_t PWM_enuInit( void )
{
	ES_t Local_enuErrorState = ES_NOK ;

	u8 Local_u8ClkSelect , Local_u8Flag = 0 ;

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < PWM_u8MaxNum ; Local_u8Iter++ )
	{
		if( PWMs[Local_u8Iter].TimerNum == TIMER1A || PWMs[Local_u8Iter].TimerNum == TIMER1B )
		{
			if( ! Local_u8Flag )
			{
				Local_u8Flag = 1 ;
				TIMSK &= ~( TC1_INT_EN_MASK ) ;				// Disable All Interrupts before setting all conditions
				TCCR1A = 0x00 ;								// Masks all bits in TCCR1A
				TCCR1B = 0x00 ;								// Masks all bits in TCCR1B
				u8 Local_u8CopySREG = SREG ;
				_CLI_;
				OCR1AH = 0x00 ;								// Clears 1A HIGH-byte Output Compare Register
				OCR1AL = 0x00 ;								// Clears 1A LOW-byte Output Compare Register
				OCR1BH = 0x00 ;								// Clears 1B HIGH-byte Output Compare Register
				OCR1BL = 0x00 ;								// Clears 1B LOW-byte Output Compare Register
				SREG = Local_u8CopySREG ;

				/****************************************/
				/*			Set Clock Selection			*/
				/****************************************/
				Global_u32Timer1_Clk = PWM_u32TimerClock( TIMER1A , PWMs[Local_u8Iter].ClkSelect ,&Local_u8ClkSelect);
				TCCR1B |= ( Local_u8ClkSelect << CLK1_SEL_BITS );

				/****************************************/
				/*		Set Waveform Generation Mode	*/
				/****************************************/
				if( PWMs[Local_u8Iter].WaveGenMode > WGM_MODE_00	&& PWMs[Local_u8Iter].WaveGenMode <= WGM_FAST_OCR1A	&&
					PWMs[Local_u8Iter].WaveGenMode != WGM_MODE_04	&& PWMs[Local_u8Iter].WaveGenMode != WGM_MODE_12	&&
					PWMs[Local_u8Iter].WaveGenMode != WGM_MODE_13	)
				{
					u8 Local_u8WGM = PWMs[Local_u8Iter].WaveGenMode - WGM_MODE_00 ;
					TCCR1A |= ( ( Local_u8WGM & _TWO_BITS_MASK_ ) << WGM1A_SEL_BITS );
					TCCR1B |= ( ( ( Local_u8WGM >> BIT1_MASK) & _TWO_BITS_MASK_ ) << WGM1B_SEL_BITS );
				}
				else
				{
					Local_enuErrorState = ES_OUT_RANGE ;
					#warning "Timer_Init(): Non Supported Waveform Gen mode for Timer1. Timer1 WGM is set to WGM_NORMAL_MODE"
				}
			}
			/****************************************/
			/*		Set Compare Output Mode			*/
			/****************************************/
			if( PWMs[Local_u8Iter].CompOutMode == COMP_NON_INVERTED ||
				PWMs[Local_u8Iter].CompOutMode == COMP_INVERTED		)
			{
				if( PWMs[Local_u8Iter].TimerNum == TIMER1A )
				{
					/****************************************/
					/*		Set 1A Compare Output Mode		*/
					/****************************************/
					TCCR1A |= ( ( PWMs[Local_u8Iter].CompOutMode - COMP_NORMAL ) << COM1A_SEL_BITS );
				}
				else
				{
					/****************************************/
					/*		Set 1B Compare Output Mode		*/
					/****************************************/
					TCCR1A |= ( ( PWMs[Local_u8Iter].CompOutMode - COMP_NORMAL ) << COM1B_SEL_BITS );
				}
			}
			else
			{
				Local_enuErrorState = ES_OUT_RANGE ;
				#warning "PWM_Init(): Non Supported Compare Output mode for Timer1. Timer1 COM mode is set to COMP_NORMAL"
			}

			/****************************************/
			/*			Set Interrupt Mode			*/
			/****************************************/
			switch( PWMs[Local_u8Iter].InterruptMode )
			{
				case PWM_OVERFLOW_INT	:	SET_BIT( TIMSK , TOIE1_BIT );				/*	Enable TOIE1 Interrupt */
											break;
				case PWM_OUT_COMP_INT	:	if( PWMs[Local_u8Iter].TimerNum == TIMER1B )
											{
												SET_BIT( TIMSK , OCIE1B_BIT );			/*	Enable OCIE1B Interrupt */
											}
											else
											{
												SET_BIT( TIMSK , OCIE1A_BIT );			/*	Enable OCIE1A Interrupt */
											}
											break;
				default :	Local_enuErrorState = ES_OUT_RANGE ;
							#warning " Timer_Init(): Non Supported Interrupt mode for Timer1. Timer1 Interrupts are Disabled"
							/*Timer Interrupts are already disabled at beginning of setting value, no extra action is needed*/
			}
		}
		else
		{
			Local_enuErrorState = ES_OUT_RANGE ;
			#warning " PWM_Init(): Non Supported Timer Number , Initialization is terminated."
			break;
		}
	}

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

static u32 PWM_u32TimerClock( u8 Copy_u8TimerNum ,u8 Copy_u8ClkSelectNum ,u8 *Copy_pu8ClkSelectPrescalar )
{
	u32 Copy_u32TimerClk = CPU_CLOCK;


	switch( Copy_u8ClkSelectNum )
	{
		case PWM_NO_CLK		:	*Copy_pu8ClkSelectPrescalar = 0;
							Copy_u32TimerClk = 0 ;
							break;
		case PWM_PRES_1		:	*Copy_pu8ClkSelectPrescalar = 1;
						break;
		case PWM_PRES_8 	:	*Copy_pu8ClkSelectPrescalar = 2;
							Copy_u32TimerClk >>= 3 ;
						break;
		case PWM_PRES_64 	:	Copy_u32TimerClk >>= 6 ;
							*Copy_pu8ClkSelectPrescalar = 3;
						break;
		case PWM_PRES_256	 :	Copy_u32TimerClk >>= 8 ;
							//if( Copy_u8TimerNum == TIMER2 )	*Copy_pu8ClkSelectPrescalar = 6;
							//else
							*Copy_pu8ClkSelectPrescalar = 4;
						break;
		case PWM_PRES_1024 :	Copy_u32TimerClk >>= 10 ;
							//if( Copy_u8TimerNum == TIMER2 )	*Copy_pu8ClkSelectPrescalar = 7;
							//else
							*Copy_pu8ClkSelectPrescalar = 5;
						break;
		case PWM_EXT_CLK_FALL	 :		Copy_u32TimerClk = EXT_CLOCK ;
									//if( Copy_u8TimerNum == TIMER0 || Copy_u8TimerNum == TIMER1A || Copy_u8TimerNum == TIMER1B || Copy_u8TimerNum == TIMER1 )
									//{
										*Copy_pu8ClkSelectPrescalar = 6;
										//break;
									//}
									//#warning "External Clock is not supported in Timer2 , NO Clock mode is selected"
						break;
		case PWM_EXT_CLK_RISE	 :		Copy_u32TimerClk = EXT_CLOCK ;
									//if( Copy_u8TimerNum == TIMER0 || Copy_u8TimerNum == TIMER1A || Copy_u8TimerNum == TIMER1B || Copy_u8TimerNum == TIMER1 )
									//{
										*Copy_pu8ClkSelectPrescalar = 7;
									//	break;
									//}
									//#warning "External Clock is not supported in Timer2 , NO Clock mode is selected"
						break;
		default:	Copy_u32TimerClk = 0;
					#warning "Unsupported Clock mode is selected , Timer is disabled"
	}

	return Copy_u32TimerClk ;

}
#ifndef PWM
ES_t PWM_enuGetClock( u8 Copy_u8TimerNum , u32 *Copy_pu32TimerClk)
{
	ES_t Local_enuErrorState = ES_NOK ;

	if( Copy_u8TimerNum == TIMER1A || Copy_u8TimerNum == TIMER1B )
		*Copy_pu32TimerClk = Global_u32Timer1_Clk;
	else Local_enuErrorState = ES_OUT_RANGE ;

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}
#endif

ES_t PWM_enuSetClkPrescaler( u8 Copy_u8TimerNum , u8 Copy_u8PrescalerValue )
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8ClkSelectPrescaler = PWM_NO_CLK ;
	u8 Local_u8CopyTIMSK = TIMSK ;									// Saving a Copy of Timer Interrupt Mask Register

	u32 Local_u32TimerClk = PWM_u32TimerClock( Copy_u8TimerNum , Copy_u8PrescalerValue , &Local_u8ClkSelectPrescaler ) ;

	if( Copy_u8TimerNum == TIMER1A || Copy_u8TimerNum == TIMER1B )
	{
		TIMSK &= ~( TC1_INT_EN_MASK ) ;								// Disable Timer1 Interrupts
		TCCR1B &= ~( CLK1_SEL_BITS_MASK );							// Masking Clock Select bits
		TCCR1B |= ( Local_u8ClkSelectPrescaler  << CLK1_SEL_BITS );	// Setting New Prescaler
		Global_u32Timer1_Clk = Local_u32TimerClk ;					// Saving New Actual Timer1 Clock
#ifndef PWM
		PWMs[ TIMER1A - TIMER1A ].ClkSelect = Copy_u8PrescalerValue ;
		PWMs[ TIMER1B - TIMER1A ].ClkSelect = Copy_u8PrescalerValue ;
#endif
	}
	else Local_enuErrorState = ES_OUT_RANGE ;

	TIMSK = Local_u8CopyTIMSK ;										// Re-setting Timer Interrupt Mask Register to its initial Status

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}
#ifndef PWM
ES_t PWM_enuGetClkSelect( u8 Copy_u8TimerNum , u16 *Copy_pu8TimerClkSelect )
{
	ES_t Local_enuErrorState = ES_OUT_RANGE ;

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < PWM_u8MaxNum ; Local_u8Iter++ )
	{
		if( PWMs[Local_u8Iter].TimerNum == Copy_u8TimerNum )
		{
			*Copy_pu8TimerClkSelect = PWMs[Local_u8Iter].ClkSelect;
			Local_enuErrorState = ES_OK ;
		}
	}


	return Local_enuErrorState ;
}
#endif
ES_t PWM_enuSetCOM_Mode( u8 Copy_u8TimerNum , u8 Copy_u8COM_Mode )
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8CopyTIMSK = TIMSK ;														// Saving a Copy of Timer Interrupt Mask Register

	if( Copy_u8COM_Mode == COMP_NON_INVERTED ||	Copy_u8COM_Mode == COMP_INVERTED )
	{
		if( Copy_u8TimerNum == TIMER1A || Copy_u8TimerNum == TIMER1B )
		{
			TIMSK &= ~( TC1_INT_EN_MASK ) ;													// Disable Timer1 Interrupts
			if( Copy_u8TimerNum == TIMER1A )
			{
				TCCR1A &= ~( COM1A_SEL_BITS_MASK ) ;										// Masking COM Select bits
				TCCR1A |= ( ( Copy_u8COM_Mode - COMP_NORMAL) << COM1A_SEL_BITS ) ;			// Setting New Compare Output Mode
#ifndef PWM
				PWMs[ TIMER1A - TIMER1A ].CompOutMode = Copy_u8COM_Mode ;
#endif
			}
			else if( Copy_u8TimerNum == TIMER1B )
			{
				TCCR1B &= ~( COM1B_SEL_BITS_MASK ) ;										// Masking COM Select bits
				TCCR1B |= ( ( Copy_u8COM_Mode - COMP_NORMAL) << COM1B_SEL_BITS ) ;			// Setting New Compare Output Mode
#ifndef PWM
				PWMs[ TIMER1B - TIMER1A ].CompOutMode = Copy_u8COM_Mode ;
#endif
			}
			else
			{
				#warning "Timer_enuSetCOM_Mode() : For setting COM mode please Specify TIMER1A / TIMER1B , TIMER1 choice is not allowed. No Action Taken. "
			}
		}
		else Local_enuErrorState = ES_OUT_RANGE ;

		TIMSK = Local_u8CopyTIMSK ;															// Re-setting Timer Interrupt Mask Register to its Status

	}
	else
	{
		Local_enuErrorState = ES_OUT_RANGE ;
		#warning "PWM_enuSetCOM_Mode(): Non Supported Compare Output mode, No Action Taken ."
	}


	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}
#ifndef PWM
ES_t PWM_enuGetCOM_Mode( u8 Copy_u8TimerNum , u8 *Copy_pu8PWM_OC_Mode)
{
	ES_t Local_enuErrorState = ES_OUT_RANGE ;

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < PWM_u8MaxNum ; Local_u8Iter++ )
	{
		if( PWMs[Local_u8Iter].TimerNum == Copy_u8TimerNum )
		{
			*Copy_pu8PWM_OC_Mode = PWMs[Local_u8Iter].CompOutMode;
			Local_enuErrorState = ES_OK ;
		}
	}

	return Local_enuErrorState ;
}
#endif

ES_t PWM_enuSetWGM_Mode( u8 Copy_u8TimerNum , u8 Copy_u8WGM_Mode )
{
	ES_t Local_enuErrorState = ES_NOK;


	if( Copy_u8WGM_Mode > WGM_MODE_00	&& Copy_u8WGM_Mode <= WGM_FAST_OCR1A &&
		Copy_u8WGM_Mode != WGM_MODE_04	&& Copy_u8WGM_Mode != WGM_MODE_12 && Copy_u8WGM_Mode != WGM_MODE_13 )
	{

		u8 Local_u8CopyTIMSK = TIMSK ;									// Saving a Copy of Timer Interrupt Mask Register

		if( ( Copy_u8TimerNum == TIMER1A || Copy_u8TimerNum == TIMER1B ) &&
			Copy_u8WGM_Mode > WGM_MODE_00	&& Copy_u8WGM_Mode <= WGM_FAST_OCR1A &&
			Copy_u8WGM_Mode != WGM_MODE_04	&& Copy_u8WGM_Mode != WGM_MODE_12 && Copy_u8WGM_Mode != WGM_MODE_13 )
		{
			u8 Local_u8WGM = Copy_u8WGM_Mode - WGM_MODE_00 ;
			TIMSK &= ~( TC1_INT_EN_MASK ) ;													// Disable Timer1 Interrupts
			TCCR1B &= ~( WGM1B_SEL_BITS_MASK );												// Masking WGM1B Select bits
			TCCR1A |= ( ( Local_u8WGM & _TWO_BITS_MASK_ ) << WGM1A_SEL_BITS );				// Setting WGM10 & WGM11 Select bits
			TCCR1B |= ( ( ( Local_u8WGM >> BIT1_MASK) & _TWO_BITS_MASK_ ) << WGM1B_SEL_BITS );// Setting WGM12 & WGM13 Select bits
			PWMs[ TIMER1A - TIMER1A ].WaveGenMode = Copy_u8WGM_Mode ;
			PWMs[ TIMER1B - TIMER1A ].WaveGenMode = Copy_u8WGM_Mode ;
		}
		else Local_enuErrorState = ES_OUT_RANGE ;

		TIMSK = Local_u8CopyTIMSK ;															// Re-setting Timer Interrupt Mask Register to its Status
	}
	else Local_enuErrorState = ES_OUT_RANGE ;

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}
#ifndef PWM
ES_t PWM_enuGetWGM_Mode( u8 Copy_u8TimerNum , u8 *Copy_pu8TimerWGM_Mode)
{
	ES_t Local_enuErrorState = ES_OUT_RANGE ;

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < PWM_u8MaxNum ; Local_u8Iter++ )
	{
		if( PWMs[Local_u8Iter].TimerNum == Copy_u8TimerNum )
		{
			*Copy_pu8TimerWGM_Mode = PWMs[Local_u8Iter].WaveGenMode;
			Local_enuErrorState = ES_OK ;
		}
	}
	return Local_enuErrorState ;
}
#endif

ES_t PWM_enuSetOCRnValue( u8 Copy_u8TimerNum , u16 Copy_u16OCRnValue )
{
	ES_t Local_enuErrorState = ES_NOK ;

	if( Copy_u8TimerNum == TIMER1A && Copy_u16OCRnValue <= Timer1Max )
	{
		u8 Local_u8Temp = SREG ;
		_CLI_;
		OCR1AH  = Copy_u16OCRnValue >> 8 ;
		OCR1AL 	= Copy_u16OCRnValue ;
		SREG = Local_u8Temp;
		Global_u16OCR1A_Value = Copy_u16OCRnValue ;
	}
	else if( Copy_u8TimerNum == TIMER1B && Copy_u16OCRnValue <= Timer1Max )
	{
		u8 Local_u8Temp = SREG ;
		_CLI_;
		OCR1BH  = Copy_u16OCRnValue >> 8 ;
		OCR1BL 	= Copy_u16OCRnValue ;
		SREG = Local_u8Temp;
		Global_u16OCR1B_Value = Copy_u16OCRnValue ;
	}
	else Local_enuErrorState = ES_OUT_RANGE ;

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}
#ifndef PWM
ES_t PWM_enuReadOCRnValue( u8 Copy_u8TimerNum , void *Copy_pCounterValue )
{
	ES_t Local_enuErrorState = ES_NOK ;

		if( Copy_u8TimerNum == TIMER1A )
		{
			*( (u16 *)Copy_pCounterValue ) = Global_u16OCR1A_Value ;
		}
		else if( Copy_u8TimerNum == TIMER1B )
		{
			*( (u16 *)Copy_pCounterValue ) = Global_u16OCR1B_Value ;
		}
		else Local_enuErrorState = ES_OUT_RANGE ;

		return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}
#endif
ES_t PWM_enuSetICR1Value( u16 Copy_u16ICR1Value )
{
	u8 Local_u8Temp = SREG ;
	_CLI_;
	ICR1H = Copy_u16ICR1Value >> 8 ;
	ICR1L = Copy_u16ICR1Value ;
	SREG = Local_u8Temp;
	Global_u16ICR1_Value = Copy_u16ICR1Value ;
	return ES_OK;
}

ES_t PWM_enuSetInterruptMode( u8 Copy_u8TimerNum , u8 Copy_u8TimerInterruptMode )
{
	ES_t Local_enuErrorState = ES_NOK ;

	if( ( Copy_u8TimerNum == TIMER1A || Copy_u8TimerNum == TIMER1B ) &&
		( Copy_u8TimerInterruptMode == PWM_OVERFLOW_INT || Copy_u8TimerInterruptMode == PWM_OUT_COMP_INT ) )
	{
		u8 Local_u8Iter = 0 , Local_u8Updated = 0 ;

		switch( Copy_u8TimerInterruptMode )
		{
			case PWM_OVERFLOW_INT	:	SET_BIT( TIMSK , TOIE1_BIT );					/*	Enable TOIE1 Interrupt */
										for( Local_u8Iter = 0 ; Local_u8Iter < PWM_u8MaxNum ; Local_u8Iter++  )
										{
											PWMs[Local_u8Iter].InterruptMode = Copy_u8TimerInterruptMode ;
										}
										Local_u8Updated = 1;
										break;
			case PWM_OUT_COMP_INT	:	if( Copy_u8TimerNum == TIMER1B )
										{
											SET_BIT( TIMSK , OCIE1B_BIT );				/*	Enable OCIE1B Interrupt */
										}
										else
										{
											SET_BIT( TIMSK , OCIE1A_BIT );				/*	Enable OCIE1A Interrupt */
										}
										break;
		}
		if( !Local_u8Updated )
		{
			for( Local_u8Iter = 0 ; Local_u8Iter < PWM_u8MaxNum ; Local_u8Iter++  )
			{
				if( PWMs[Local_u8Iter].TimerNum == Copy_u8TimerNum )
				{
					PWMs[Local_u8Iter].InterruptMode = Copy_u8TimerInterruptMode ;
				}
			}
		}
	}
	else Local_enuErrorState = ES_OUT_RANGE ;

	return Local_enuErrorState ;
}

ES_t PWM_enuGetInterruptMode( u8 Copy_u8TimerNum , u8 *Copy_pu8TimerInterruptMode )
{
	ES_t Local_enuErrorState = ES_NOK;

	if( Copy_pu8TimerInterruptMode != NULL )
	{
		if( Copy_u8TimerNum == TIMER1A	|| Copy_u8TimerNum == TIMER1A )
		{
			for( u8 Local_u8Iter = 0 ; Local_u8Iter < PWM_u8MaxNum ; Local_u8Iter++  )
			{
				if( PWMs[Local_u8Iter].TimerNum == Copy_u8TimerNum )
				{
					*Copy_pu8TimerInterruptMode = PWMs[Local_u8Iter].InterruptMode ;
				}
			}
		}
		else Local_enuErrorState = ES_OUT_RANGE ;
	}
	else Local_enuErrorState = ES_NULL_POINTER ;

	return Local_enuErrorState;
}

ES_t PWM_enuSetDutyCycle( u8 Copy_u8TimerNum , f32 Copy_f32DutyCycle )
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8TimerWGM_Mode , Local_u8TimerCOM_Mode , Local_u8Flag = 0 ;
	u16 Local_u16TimerTop , Local_u16OCRValue ;

	if( Copy_u8TimerNum == TIMER1A || Copy_u8TimerNum == TIMER1B )
	{
		for(u8 Local_u8Iter = 0 ; Local_u8Iter < PWM_u8MaxNum ; Local_u8Iter++ )
		{
			if( PWMs[Local_u8Iter].TimerNum == Copy_u8TimerNum )
			{
				Local_u8TimerWGM_Mode = PWMs[Local_u8Iter].WaveGenMode ;
				Local_u8TimerCOM_Mode = PWMs[Local_u8Iter].CompOutMode ;
			}
		}
		switch( Local_u8TimerWGM_Mode )
		{
			case WGM_PC_8_bit	:	Local_u16TimerTop = EIGHT_BIT_TOP ;
									PC_OCR_CALCULATOR;
									break;
			case WGM_FAST_8_bit	:	Local_u16TimerTop = EIGHT_BIT_TOP ;
									FAST_OCR_CALCULATOR;
									break;
			case WGM_PC_9_bit	:	Local_u16TimerTop = NINE_BIT_TOP ;
									PC_OCR_CALCULATOR;
									break;
			case WGM_FAST_9_bit	:	Local_u16TimerTop = NINE_BIT_TOP ;
									FAST_OCR_CALCULATOR;
									break;
			case WGM_PC_10_bit	:	Local_u16TimerTop = TEN_BIT_TOP ;
									PC_OCR_CALCULATOR;
									break;
			case WGM_FAST_10_bit:	Local_u16TimerTop = TEN_BIT_TOP ;
									FAST_OCR_CALCULATOR;
									break;
			case WGM_PFC_ICR1	:	Local_u16TimerTop = Global_u16ICR1_Value ;
									PC_OCR_CALCULATOR ;
									break;
			case WGM_PC_ICR1	:	Local_u16TimerTop = Global_u16ICR1_Value ;
									PC_OCR_CALCULATOR;
									break;
			case WGM_FAST_ICR1	:	Local_u16TimerTop = Global_u16ICR1_Value ;
									FAST_OCR_CALCULATOR;
									break;
			case WGM_PFC_OCR1A	:	Local_u16TimerTop = Global_u16OCR1A_Value ;
									PC_OCR_CALCULATOR;
									break;
			case WGM_PC_OCR1A	:	Local_u16TimerTop = Global_u16OCR1A_Value ;
									PC_OCR_CALCULATOR;
									break;
			case WGM_FAST_OCR1A	:	Local_u16TimerTop = Global_u16OCR1A_Value ;
									FAST_OCR_CALCULATOR;
									break;
		}

		if( !Local_u8Flag )
		{
			if( Copy_u8TimerNum == TIMER1A )
			{
				u8 Local_u8Temp = SREG ;
				_CLI_;
				OCR1AH  = Local_u16OCRValue >> _BYTE_SHIFT_ ;
				OCR1AL 	= Local_u16OCRValue ;
				SREG = Local_u8Temp;
				Global_u16OCR1A_Value = Local_u16OCRValue ;
			}
			else if( Copy_u8TimerNum == TIMER1B )
			{
				u8 Local_u8Temp = SREG ;
				_CLI_;
				OCR1BH  = Local_u16OCRValue >> _BYTE_SHIFT_ ;
				OCR1BL 	= Local_u16OCRValue ;
				SREG = Local_u8Temp;
				Global_u16OCR1B_Value = Local_u16OCRValue ;
			}
		}
		else
		{
			Local_enuErrorState = ES_OUT_RANGE;
			#warning " PWM_enuSetDutyCycle(): Requested Duty Cycle Can NOT be reached in this mode "
		}
	}
	else Local_enuErrorState = ES_OUT_RANGE ;

	return ( Local_enuErrorState == ES_NOK ? ES_OK : Local_enuErrorState ) ;
}

ES_t PWM_enuInterruptEnable( u8 Copy_u8TimerIntName)
{
	ES_t Local_enuErrorState = ES_NOK;

	_CLI_;														// Disable all Interrupts

	switch ( Copy_u8TimerIntName )
	{
		case TOIE1	:	SET_BIT( TIMSK , TOIE1_BIT);
						break;
		case OCIE1B	:	SET_BIT( TIMSK , OCIE1B_BIT);
						break;
		case OCIE1A	:	SET_BIT( TIMSK , OCIE1A_BIT);
						break;
		default		:	Local_enuErrorState = ES_OUT_RANGE;
						#warning " Timer_enuInterruptEnable() : Unidentified Interrupt Name, No Action Taken."
	}
	_SEI_;													// Enable ALL Interrupts

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t PWM_enuInterruptDisable( u8 Copy_u8TimerIntName)
{
	ES_t Local_enuErrorState = ES_NOK;

	_CLI_;																// Disable ALL Interrupts

	switch ( Copy_u8TimerIntName )
	{
		case TOIE1	:	CLR_BIT( TIMSK , TOIE1_BIT);
						break;
		case OCIE1B	:	CLR_BIT( TIMSK , OCIE1B_BIT);
						break;
		case OCIE1A	:	CLR_BIT( TIMSK , OCIE1A_BIT);
						break;
		default		:	Local_enuErrorState = ES_OUT_RANGE;
						#warning " Timer_enuInterruptEnable() : Unidentified Interrupt Name, No Action Taken."
	}

	_SEI_;																// Re-Enable Global Interrupt

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}
#ifndef PWM
bool PWM_IsInterruptEnabled( u8 Copy_u8TimerIntName )
{
	u8 Local_u8Status = 0 ;

	switch ( Copy_u8TimerIntName )
	{
		case TOIE1	: 	Local_u8Status = IS_BIT( TIMSK , TOIE1_BIT);
						break;
		case OCIE1B	:	Local_u8Status = IS_BIT( TIMSK , OCIE1B_BIT);
						break;
		case OCIE1A	:	Local_u8Status = IS_BIT( TIMSK , OCIE1A_BIT);
						break;
	}
	return ( ( Local_u8Status )? TRUE : FALSE ) ;
}
#endif

ES_t PWM_enuCallBack( u8 Copy_u8TimerIntName , void (*Copy_pAppFun)(void) )
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 found = 0 ;

	if( Copy_pAppFun != NULL )
	{
		for( u8 Local_u8Iter; Local_u8Iter < PWM_INTERRUPTS ; Local_u8Iter++ )
		{
			if( Global_AstrPWM_Pointers[Local_u8Iter].InterruptName == Copy_u8TimerIntName )
			{
				Global_AstrPWM_Pointers[Local_u8Iter].ptrFun = Copy_pAppFun;
				Local_enuErrorState = ES_OK ;
				found = 1;
				break;
			}
		}
		if(!found) Local_enuErrorState = ES_OUT_RANGE ;
	}
	else Local_enuErrorState = ES_NULL_POINTER;

	return Local_enuErrorState ;
}

void __vector_7( void )__attribute__((signal));		/*	Timer1	Compare Match A Interrupt ISR	*/
void __vector_8( void )__attribute__((signal));		/*	Timer1	Compare Match B Interrupt ISR	*/
void __vector_9( void )__attribute__((signal));		/*	Timer1	Overflow Interrupt	ISR			*/


void __vector_7( void )/*	OCIE1A	*/
{
	if( Global_AstrPWM_Pointers[2].ptrFun != NULL )
	{
		(*Global_AstrPWM_Pointers[2].ptrFun)( );
	}
}
void __vector_8( void )/*	OCIE1B	*/
{
	if( Global_AstrPWM_Pointers[1].ptrFun != NULL )
	{
		(*Global_AstrPWM_Pointers[1].ptrFun)( );
	}
}
void __vector_9( void )/*	TOIE1	*/
{
	if( Global_AstrPWM_Pointers[0].ptrFun != NULL )
	{
		(*Global_AstrPWM_Pointers[0].ptrFun)( );
	}
}
