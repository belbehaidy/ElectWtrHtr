/*
 * TIMER_pro.c
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

static u32 Timer_u32TimerClock( u8 Copy_u8TimerNum ,u8 Copy_u8ClkSelectNum ,u8 *Copy_pu8ClkSelectPrescalar );

extern u8 Timers_u8MaxNum , Timer0Max , Timer2Max ;
extern Timer_t	Timers[];

static u8 Global_u8OCR0_Value = 0x00  , Global_u8OCR2_Value = 0x00 ;
static u32 Global_u32Timer0_Clk , Global_u32Timer2_Clk ;



TimerInt_Pointers_t Global_AstrTimerPointers[TIMERS_INTERRUPTS] =	{
																		{ TOIE0 ,	NULL},
																		{ OCIE0 ,	NULL},
																		{ TOIE2 ,	NULL},
																		{ OCIE2 ,	NULL},
																	};


ES_t Timer_enuInit( void )
{
	ES_t Local_enuErrorState = ES_NOK ;

	u8 Local_u8ClkSelect ;

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < Timers_u8MaxNum ; Local_u8Iter++ )
	{
		Global_u32Timer0_Clk = Timer_u32TimerClock( Timers[Local_u8Iter].TimerNum , Timers[Local_u8Iter].ClkSelect ,&Local_u8ClkSelect);
		if( Timers[Local_u8Iter].TimerNum == TIMER0)
		{
			TIMSK &= ~( TC0_INT_EN_MASK ) ;			// Disable All Interrupts before setting all conditions
			TCCR0 = 0x00 ;							// Masks all bits in TCCR0
			OCR0  = 0x00 ;							// Clears Output Compare Register
			/*	Set Clock Selection	*/
//			Global_u32Timer0_Clk = Timer_u32TimerClock( TIMER0 , Timers[Local_u8Iter].ClkSelect ,&Local_u8ClkSelect);
			TCCR0 |= ( Local_u8ClkSelect  << CLK0_SEL_BITS );
			/*	Set Compare Output Mode	*/
			TCCR0 |= ( ( Timers[Local_u8Iter].CompOutMode - COMP_NORMAL ) << COMP0_MAT_OUT_MODE_BITS );
			/*	Set Waveform Generation Mode	*/
			if( Timers[Local_u8Iter].WaveGenMode == WGM_NORMAL_MODE	)
			{
				//NO ACTION NEEDED
			}
			else if( Timers[Local_u8Iter].WaveGenMode == WGM_CTC_MODE )
			{
				SET_BIT( TCCR0 , WGM01_BIT ) ;
			}
			else
			{
				Local_enuErrorState = ES_OUT_RANGE ;
				#warning "Timer_Init(): Non Supported Waveform Gen mode for Timer0. Timer0 WGM is set to WGM_NORMAL_MODE"
			}
			/*	Set Interrupt Mode	*/
			switch( Timers[Local_u8Iter].InterruptMode )
			{
				case TC_OVERFLOW_INT	:	SET_BIT( TIMSK , TOIE0_BIT );	/*	Enable TOIE0 Interrupt */
											break;
				case TC_OUT_COMP_INT	:	SET_BIT( TIMSK , OCIE0_BIT );	/*	Enable OCIE0 Interrupt */
											break;

				default :	Local_enuErrorState = ES_OUT_RANGE ;
							#warning " Timer_Init(): Non Supported Interrupt mode for Timer0. Timer0 Interrupts are Disabled"
							/*Timer Interrupts are already disabled at beginning of setting value, no extra action is needed*/
			}
		}
		else if( Timers[Local_u8Iter].TimerNum == TIMER2)
		{
			TIMSK &= ~( TC2_INT_EN_MASK ) ;			// Disable All Interrupts before setting all conditions
			TCCR2 = 0x00 ;							// Masks all bits in TCCR2
			OCR2  = 0x00 ;							// Clears Output Compare Register

			/*	Set Clock Selection	*/
//			Global_u32Timer2_Clk = Timer_u32TimerClock( TIMER2 , Timers[Local_u8Iter].ClkSelect ,&Local_u8ClkSelect);
			TCCR2 |= ( Local_u8ClkSelect << CLK2_SEL_BITS );
			/*	Set Compare Output Mode	*/
			TCCR2 |= ( ( Timers[Local_u8Iter].CompOutMode - COMP_NORMAL ) << COMP2_MAT_OUT_MODE_BITS );
			/*	Set Waveform Generation Mode	*/
			if( Timers[Local_u8Iter].WaveGenMode == WGM_NORMAL_MODE )
			{
				// NO ACTION NEEDED
			}
			else if(Timers[Local_u8Iter].WaveGenMode == WGM_CTC_MODE )
			{
				SET_BIT( TCCR2 , WGM21_BIT ) ;
			}
			else
			{
				Local_enuErrorState = ES_OUT_RANGE ;
				#warning "Timer_Init(): Non Supported Waveform Gen mode for Timer2. Timer2 WGM is set to WGM_NORMAL_MODE"
			}
			/*	Set Interrupt Mode	*/
			switch( Timers[Local_u8Iter].InterruptMode )
			{
				case TC_OVERFLOW_INT	:	SET_BIT( TIMSK , TOIE2_BIT );	/*	Enable TOIE2 Interrupt */
											break;
				case TC_OUT_COMP_INT	:	SET_BIT( TIMSK , OCIE2_BIT );	/*	Enable OCIE2 Interrupt */
											break;

				default :	Local_enuErrorState = ES_OUT_RANGE ;
							#warning " Timer_Init(): Non Supported Interrupt mode for Timer2. Timer2 Interrupts are Disabled"
							/*Timer Interrupts are already disabled at beginning of setting value, no extra action is needed*/
			}
		}
		else
		{
			Local_enuErrorState = ES_OUT_RANGE ;
			#warning " Timer_Init(): Non Supported Timer Number , Initialization is terminated."
			break;
		}
	}

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

static u32 Timer_u32TimerClock( u8 Copy_u8TimerNum ,u8 Copy_u8ClkSelectNum ,u8 *Copy_pu8ClkSelectPrescalar )
{
	u32 Copy_u32TimerClk = CPU_CLOCK;


	switch( Copy_u8ClkSelectNum )
	{
		case NO_CLK		:	*Copy_pu8ClkSelectPrescalar = 0;
							Copy_u32TimerClk = 0 ;
							break;
		case PRES_1		:	*Copy_pu8ClkSelectPrescalar = 1;
						break;
		case PRES_8 	:	*Copy_pu8ClkSelectPrescalar = 2;
							Copy_u32TimerClk >>= 3 ;
						break;
		case PRES_32 	:	if( Copy_u8TimerNum == TIMER2 )
							{
								*Copy_pu8ClkSelectPrescalar = 3;
								Copy_u32TimerClk >>= 5 ;
								break;
							}
							#warning "Clock Select Number selected is not supported in Timer0/Timer1 , shifted to PRES_64"
							/*	NO BREAK IN CASE OF NOT TIMER2	*/
		case PRES_64 	:	Copy_u32TimerClk >>= 6 ;
							if( Copy_u8TimerNum == TIMER2 )	*Copy_pu8ClkSelectPrescalar = 4;
							else *Copy_pu8ClkSelectPrescalar = 3;
						break;
		case PRES_128 	:	if( Copy_u8TimerNum == TIMER2 )
							{
								*Copy_pu8ClkSelectPrescalar = 5;
								Copy_u32TimerClk >>= 7 ;
								break;
							}
							#warning "Clock Select Number selected is not supported in Timer0/Timer1 , shifted to PRES_256"
							/*	NO BREAK IN CASE OF NOT TIMER2	*/
		case PRES_256	 :	Copy_u32TimerClk >>= 8 ;
							if( Copy_u8TimerNum == TIMER2 )	*Copy_pu8ClkSelectPrescalar = 6;
							else *Copy_pu8ClkSelectPrescalar = 4;
						break;
		case PRES_1024 :	Copy_u32TimerClk >>= 10 ;
							if( Copy_u8TimerNum == TIMER2 )	*Copy_pu8ClkSelectPrescalar = 7;
							else *Copy_pu8ClkSelectPrescalar = 5;
						break;
		case EXT_CLK_FALL	 :		Copy_u32TimerClk = EXT_CLOCK ;
									if( Copy_u8TimerNum == TIMER0 )
									{
										*Copy_pu8ClkSelectPrescalar = 6;
										break;
									}
									#warning "External Clock is not supported in Timer2 , NO Clock mode is selected"
						break;
		case EXT_CLK_RISE	 :		Copy_u32TimerClk = EXT_CLOCK ;
									if( Copy_u8TimerNum == TIMER0 )
									{
										*Copy_pu8ClkSelectPrescalar = 7;
										break;
									}
									#warning "External Clock is not supported in Timer2 , NO Clock mode is selected"
						break;
		default:	Copy_u32TimerClk = 0;
					#warning "Unsupported Clock mode is selected , Timer is disabled"
	}

	return Copy_u32TimerClk ;

}
#if Mask
ES_t Timer_enuGetClock( u8 Copy_u8TimerNum , u32 *Copy_pu32TimerClk)
{
	ES_t Local_enuErrorState = ES_NOK ;

	if( Copy_u8TimerNum == TIMER0 )
		*Copy_pu32TimerClk = Global_u32Timer0_Clk;
	else if( Copy_u8TimerNum == TIMER2 )
		*Copy_pu32TimerClk = Global_u32Timer2_Clk;
	else Local_enuErrorState = ES_OUT_RANGE ;

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}
#endif

ES_t Timer_enuSetClkPrescaler( u8 Copy_u8TimerNum , u8 Copy_u8PrescalerValue )
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8ClkSelectPrescaler = NO_CLK ;
	u8 Local_u8CopyTIMSK = TIMSK ;									// Saving a Copy of Timer Interrupt Mask Register

	u32 Local_u32TimerClk = Timer_u32TimerClock( Copy_u8TimerNum , Copy_u8PrescalerValue , &Local_u8ClkSelectPrescaler ) ;

	if( Copy_u8TimerNum == TIMER0 )
	{
		TIMSK &= ~( TC0_INT_EN_MASK ) ;								// Disable Timer0 Interrupts
		TCCR0 &= ~( CLK0_SEL_BITS_MASK ) ;							// Masking Clock Select bits
		TCCR0 |= ( Local_u8ClkSelectPrescaler  << CLK0_SEL_BITS ) ;	// Setting New Prescaler
		Global_u32Timer0_Clk = Local_u32TimerClk ;					// Saving New Actual Timer0 Clock
#if Mask
		Timers[ TIMER0 - TIMER0 ].ClkSelect = Copy_u8PrescalerValue ;
#endif
	}
	else if( Copy_u8TimerNum == TIMER2 )
	{
		TIMSK &= ~( TC2_INT_EN_MASK ) ;								// Disable Timer2 Interrupts
		TCCR2 &= ~( CLK2_SEL_BITS_MASK );							// Masking Clock Select bits
		TCCR2 |= ( Local_u8ClkSelectPrescaler  << CLK2_SEL_BITS );	// Setting New Prescaler
		Global_u32Timer2_Clk = Local_u32TimerClk ;					// Saving New Actual Timer2 Clock
#if Mask
		Timers[ TIMER2 - TIMER0 ].ClkSelect = Copy_u8PrescalerValue ;
#endif
	}
	else Local_enuErrorState = ES_OUT_RANGE ;

	TIMSK = Local_u8CopyTIMSK ;										// Re-setting Timer Interrupt Mask Register to its Status

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}
#if Mask
ES_t Timer_enuGetClkSelect( u8 Copy_u8TimerNum , u16 *Copy_pu8TimerClkSelect )
{
	ES_t Local_enuErrorState = ES_OUT_RANGE ;

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < Timers_u8MaxNum ; Local_u8Iter++ )
	{
		if( Timers[Local_u8Iter].TimerNum == Copy_u8TimerNum )
		{
			*Copy_pu8TimerClkSelect = Timers[Local_u8Iter].ClkSelect;
			Local_enuErrorState = ES_OK ;
		}
	}
	return Local_enuErrorState ;
}
#endif
ES_t Timer_enuSetOCn_Mode( u8 Copy_u8TimerNum , u8 Copy_u8COM_Mode )
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8CopyTIMSK = TIMSK ;														// Saving a Copy of Timer Interrupt Mask Register

	if( Copy_u8TimerNum == TIMER0 )
	{
		TIMSK &= ~( TC0_INT_EN_MASK ) ;													// Disable Timer0 Interrupts
		TCCR0 &= ~( COMP0_MAT_OUT_MODE_BITS_MASK ) ;									// Masking COM Select bits
		TCCR0 |= ( ( Copy_u8COM_Mode - COMP_NORMAL) << COMP0_MAT_OUT_MODE_BITS ) ;		// Setting New Compare Output Mode
#if Mask
		Timers[ TIMER0 - TIMER0 ].CompOutMode = Copy_u8COM_Mode ;
#endif
	}
	else if( Copy_u8TimerNum == TIMER2 )
	{
		TIMSK &= ~( TC2_INT_EN_MASK ) ;													// Disable Timer2 Interrupts
		TCCR2 &= ~( COMP2_MAT_OUT_MODE_BITS_MASK ) ;									// Masking COM Select bits
		TCCR2 |= ( ( Copy_u8COM_Mode - COMP_NORMAL) << COMP2_MAT_OUT_MODE_BITS ) ;		// Setting New Compare Output Mode
#if Mask
		Timers[ TIMER2 - TIMER0 ].CompOutMode = Copy_u8COM_Mode ;
#endif
	}
	else Local_enuErrorState = ES_OUT_RANGE ;

	TIMSK = Local_u8CopyTIMSK ;															// Re-setting Timer Interrupt Mask Register to its Status

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}
#if Mask
ES_t Timer_enuGetOCn_Mode( u8 Copy_u8TimerNum , u8 *Copy_pu8TimerCOM_Mode)
{
	ES_t Local_enuErrorState = ES_OUT_RANGE ;

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < Timers_u8MaxNum ; Local_u8Iter++ )
	{
		if( Timers[Local_u8Iter].TimerNum == Copy_u8TimerNum )
		{
			*Copy_pu8TimerCOM_Mode = Timers[Local_u8Iter].CompOutMode;
			Local_enuErrorState = ES_OK ;
		}
	}

	return Local_enuErrorState ;
}
#endif

ES_t Timer_enuSetTimer_Mode( u8 Copy_u8TimerNum , u8 Copy_u8WGM_Mode )
{
	ES_t Local_enuErrorState = ES_NOK;
	u8 Local_u8IntMask , Local_u8TCCR , Local_u8WGM_CTC_Bit ;
	bool ErrorFlag = FALSE ;


	if( Copy_u8WGM_Mode == WGM_NORMAL_MODE || Copy_u8WGM_Mode == WGM_CTC_MODE )
	{
		u8 Local_u8CopyTIMSK = TIMSK ;														// Saving a Copy of Timer Interrupt Mask Register

		switch( Copy_u8TimerNum )
		{
			case TIMER0 : 	Local_u8IntMask = TC0_INT_EN_MASK ;
							Local_u8TCCR = TCCR0 ;
							Local_u8WGM_CTC_Bit = WGM01_BIT;
							break;
			case TIMER2 : 	Local_u8IntMask = TC2_INT_EN_MASK ;
							Local_u8TCCR = TCCR2 ;
							Local_u8WGM_CTC_Bit = WGM21_BIT ;
							break;
			default 	: 	Local_enuErrorState = ES_OUT_RANGE ;
							ErrorFlag = TRUE;
		}
		if( ErrorFlag == FALSE )
		{
			TIMSK &= ~( Local_u8IntMask );													// Clearing Timer Interrupts while changing Timer mode
			switch ( Copy_u8WGM_Mode )
			{
				case WGM_NORMAL_MODE 	: 	CLR_BIT( Local_u8TCCR , Local_u8WGM_CTC_Bit ) ;
											break;
				case WGM_CTC_MODE 		: 	SET_BIT( Local_u8TCCR , Local_u8WGM_CTC_Bit ) ;
											break;
			}
		}
		TIMSK = Local_u8CopyTIMSK ;															// Re-setting Timer Interrupt Mask Register to its Original Status
		Timers[ Copy_u8TimerNum - TIMER0 ].WaveGenMode = Copy_u8WGM_Mode ;
	}
	else Local_enuErrorState = ES_OUT_RANGE ;

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}
#if Mask
ES_t Timer_enuGetTimer_Mode( u8 Copy_u8TimerNum , u8 *Copy_pu8TimerWGM_Mode)
{
	ES_t Local_enuErrorState = ES_OUT_RANGE ;

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < Timers_u8MaxNum ; Local_u8Iter++ )
	{
		if( Timers[Local_u8Iter].TimerNum == Copy_u8TimerNum )
		{
			*Copy_pu8TimerWGM_Mode = Timers[Local_u8Iter].WaveGenMode;
			Local_enuErrorState = ES_OK ;
		}
	}
	return Local_enuErrorState ;
}
#endif
ES_t Timer_enuReset( u8 Copy_u8TimerNum )
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8Temp = SREG ;									// Saving a Copy of AVR Status Register
	_CLI_;														// Disable All Interrupts while writing to Counter Register

	if( Copy_u8TimerNum == TIMER0 )
	{
		TCNT0 = 0x00 ;
	}
	else if( Copy_u8TimerNum == TIMER2 )
	{
		TCNT2 = 0x00 ;
	}
	else Local_enuErrorState = ES_OUT_RANGE;

	SREG = Local_u8Temp;										// Re-setting AVR Status Register to its Status

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t Timer_enuPreLoad( u8 Copy_u8TimerNum , u8 Copy_u8PreLoad)
{
	ES_t Local_enuErrorState = ES_NOK;

	if( Copy_u8TimerNum == TIMER0 )
	{
		TCNT0 = (u8)Copy_u8PreLoad ;
	}
	else if( Copy_u8TimerNum == TIMER2 )
	{
		TCNT2 = (u8)Copy_u8PreLoad ;
	}
	else Local_enuErrorState = ES_OUT_RANGE;

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t Timer_enuReadCounterValue( u8 Copy_u8TimerNum , void *Copy_pCounterValue )
{
	ES_t Local_enuErrorState = ES_NOK;

	if( Copy_pCounterValue != NULL )
	{
		if( Copy_u8TimerNum == TIMER0 )
		{
			*( (u8 *)Copy_pCounterValue ) = TCNT0 ;
		}
		else if( Copy_u8TimerNum == TIMER2 )
		{
			*( (u8 *)Copy_pCounterValue ) = TCNT2 ;
		}
		else Local_enuErrorState = ES_OUT_RANGE ;
	}
	else Local_enuErrorState = ES_NULL_POINTER ;

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t Timer_enuSetOCRnValue( u8 Copy_u8TimerNum , u8 Copy_u8OCRnValue )
{
	ES_t Local_enuErrorState = ES_NOK ;

	if( Copy_u8TimerNum == TIMER0 && Copy_u8OCRnValue <= Timer0Max )
	{
		OCR0 = Copy_u8OCRnValue ;
		Global_u8OCR0_Value = OCR0 ;
	}
	else if( Copy_u8TimerNum == TIMER2 && Copy_u8OCRnValue <= Timer2Max )
	{
		OCR2 = Copy_u8OCRnValue ;
		Global_u8OCR2_Value = OCR2 ;
	}
	else Local_enuErrorState = ES_OUT_RANGE ;

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t Timer_enuReadOCRnValue( u8 Copy_u8TimerNum , void *Copy_pCounterValue )
{
	ES_t Local_enuErrorState = ES_NOK ;

		if( Copy_u8TimerNum == TIMER0 )
		{
			*( (u8 *)Copy_pCounterValue ) = Global_u8OCR0_Value ;
		}
		else if( Copy_u8TimerNum == TIMER2 )
		{
			*( (u8 *)Copy_pCounterValue ) = Global_u8OCR2_Value ;
		}
		else Local_enuErrorState = ES_OUT_RANGE ;

		return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;

}

#if Mask
ES_t Timer_PollingDelay(u8 Copy_u8TimerNum ,u16 Copy_u16Delay_ms )
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8COMmode = 0 ;
	u32 Local_u32OverFlowCounts ;

	for( u8 Local_u8Iter = 0 ; Local_u8Iter < Timers_u8MaxNum ; Local_u8Iter++ )
	{
		if( Timers[Local_u8Iter].TimerNum == Copy_u8TimerNum )
			{
				Local_u8COMmode = Timers[Local_u8Iter].CompOutMode;
				break;
			}
	}

	if( Copy_u8TimerNum == TIMER0)
	{
		if( Global_u32Timer0_Clk )
		{
			switch( Local_u8COMmode )
			{
				case  COMP_NORMAL	:	Local_u32OverFlowCounts = ( ( ( ( Copy_u16Delay_ms - 1 ) * (u64)Global_u32Timer0_Clk ) / ( 1000UL ) )
																										+ TIMER0_MAX ) / ( TIMER0_MAX + 1 );	/*	Total Overflows	*/
										TCNT0 = ( TIMER0_MAX + 1 ) -
												( ( ( ( Copy_u16Delay_ms - 1 ) * (u64)Global_u32Timer0_Clk ) / ( 1000UL ) ) % (u16)( TIMER0_MAX + 1 ) );	/* PRE-LOAD value	*/									/*	PreLoad Value	*/

										while( Local_u32OverFlowCounts > TIMER0_BOTTOM )									/*	Delay LOOP							*/
										{
											WAIT_TILL_BIT_IS_SET( TIFR , TOV0_BIT );										/*	Waiting for Timer0 Overflow flag	*/
											Local_u32OverFlowCounts--;
											SET_BIT( TIFR , TOV0_BIT);														/*	Clearing Timer0 Overflow flag		*/
										}
										break;

			case  COMP_TOG_ON_MATCH	:	Local_u32OverFlowCounts = ( ( ( ( Copy_u16Delay_ms - 1 ) * (u64)Global_u32Timer0_Clk ) / (2 * 1000UL ) )
																				+ Global_u8OCR0_Value ) / (u16)( Global_u8OCR0_Value + 1 ) ;				/*	Total Overflows	*/
										TCNT0 = ( Global_u8OCR0_Value + 1 ) -
												( ( ( ( Copy_u16Delay_ms - 1 ) * (u64)Global_u32Timer0_Clk ) / ( 2* 1000UL ) ) % (u16)( Global_u8OCR0_Value + 1 ) );/*	PreLoad Value	*/

										while( Local_u32OverFlowCounts > TIMER0_BOTTOM )									/*	Delay LOOP							*/
										{
											WAIT_TILL_BIT_IS_SET( TIFR , OCF0_BIT );										/*	Waiting for Timer0 OutCompare flag	*/
											Local_u32OverFlowCounts--;
											SET_BIT( TIFR , OCF0_BIT);														/*	Clearing Timer0 OutCompare flag		*/
										}
										break;
#if Mask
			case  COMP_NON_INVERTED	:	/*	Same as COMP_INVERTED case	*/
			case  COMP_INVERTED		:	Local_u32OverFlowCounts = ( ( ( ( Copy_u16Delay_ms - 1 ) * (u64)Global_u32Timer0_Clk ) / ( 1000UL ) )
																				+ Global_u8OCR0_Value ) / ( Global_u8OCR0_Value + 1 );						/*	Total Overflows	*/
										TCNT0 = ( Global_u8OCR0_Value + 1 ) -
												( ( ( ( Copy_u16Delay_ms - 1 ) * (u64)Global_u32Timer0_Clk ) / ( 1000UL ) ) % (u16)( Global_u8OCR0_Value + 1 ) );	/*	PreLoad Value	*/

										while( Local_u32OverFlowCounts > TIMER0_BOTTOM )									/*	Delay LOOP								*/
										{
											WAIT_TILL_BIT_IS_SET( TIFR , OCF0 );											/*	Waiting for Timer0 Output Compare flag	*/
											Local_u32OverFlowCounts--;
											SET_BIT( TIFR , OCF0_BIT);														/*	Clearing Timer0 Overflow flag			*/
										}
										break;
#endif
			default		: Local_enuErrorState = ES_OUT_RANGE;
										break;
			}
		}
	}
	else if( Copy_u8TimerNum == TIMER2 )
	{
		if( Global_u32Timer2_Clk )
		{
			switch( Local_u8COMmode )
			{
			case  COMP_NORMAL		:	Local_u32OverFlowCounts = ( ( ( ( Copy_u16Delay_ms - 1 ) * (u64)Global_u32Timer2_Clk ) / ( 1000UL ) )
																										+ TIMER2_MAX ) / ( TIMER2_MAX + 1 );	/*	Total Overflows	*/
										TCNT2 = ( TIMER2_MAX + 1 ) -
												( ( ( ( Copy_u16Delay_ms - 1 ) * (u64)Global_u32Timer2_Clk ) / ( 1000UL ) ) % (u16)( TIMER2_MAX + 1 ) );	/* PRE-LOAD value	*/									/*	PreLoad Value	*/

										while( Local_u32OverFlowCounts > TIMER2_BOTTOM )									/*	Delay LOOP							*/
										{
											WAIT_TILL_BIT_IS_SET( TIFR , TOV2_BIT );										/*	Waiting for Timer2 Overflow flag	*/
											Local_u32OverFlowCounts--;
											SET_BIT( TIFR , TOV2_BIT);														/*	Clearing Timer2 Overflow flag		*/
										}
										break;

			case  COMP_TOG_ON_MATCH	:	Local_u32OverFlowCounts = ( ( ( ( Copy_u16Delay_ms - 1 ) * (u64)Global_u32Timer2_Clk ) / (2 * 1000UL ) )
																				+ Global_u8OCR2_Value ) / (u16)( Global_u8OCR2_Value + 1 ) ;				/*	Total Overflows	*/
										TCNT2 = ( Global_u8OCR2_Value + 1 ) -
												( ( ( ( Copy_u16Delay_ms - 1 ) * (u64)Global_u32Timer2_Clk ) / ( 2* 1000UL ) ) % (u16)( Global_u8OCR2_Value + 1 ) );/*	PreLoad Value	*/

										while( Local_u32OverFlowCounts > TIMER2_BOTTOM )									/*	Delay LOOP								*/
										{
											WAIT_TILL_BIT_IS_SET( TIFR , OCF2_BIT );										/*	Waiting for Timer2 Output Compare flag	*/
											Local_u32OverFlowCounts--;
											SET_BIT( TIFR , OCF2_BIT);														/*	Clearing Timer2 Overflow flag			*/
										}
										break;
#if Mask
			case  COMP_NON_INVERTED	:	/*	Same as COMP_INVERTED case	*/
			case  COMP_INVERTED		:	Local_u32OverFlowCounts = ( ( ( ( Copy_u16Delay_ms - 1 ) * (u64)Global_u32Timer2_Clk ) / ( 1000UL ) )
																				+ Global_u8OCR2_Value ) / ( Global_u8OCR2_Value + 1 );						/*	Total Overflows	*/
										TCNT2 = ( Global_u8OCR2_Value + 1 ) -
												( ( ( ( Copy_u16Delay_ms - 1 ) * (u64)Global_u32Timer2_Clk ) / ( 1000UL ) ) % (u16)( Global_u8OCR2_Value + 1 ) );	/*	PreLoad Value	*/

										while( Local_u32OverFlowCounts > TIMER2_BOTTOM )
										{
											WAIT_TILL_BIT_IS_SET( TIFR , OCF2_BIT );										/*	Waiting for Timer2 Output Compare flag	*/
											Local_u32OverFlowCounts--;
											SET_BIT( TIFR , OCF2_BIT);														/*	Clearing Timer2 Overflow flag			*/
										}
										break;
#endif
			default		: Local_enuErrorState = ES_OUT_RANGE;
										break;
			}
		}

	}
	else Local_enuErrorState = ES_OUT_RANGE;

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}
#endif
ES_t Timer_enuInterruptEnable( u8 Copy_u8TimerIntName)
{
	ES_t Local_enuErrorState = ES_NOK;

	_CLI_ ;																// Disable ALL Interrupts

	switch ( Copy_u8TimerIntName )
	{
		case TOIE0	:	SET_BIT( TIMSK , TOIE0_BIT);
						break;
		case OCIE0	:	SET_BIT( TIMSK , OCIE0_BIT);
						break;
		case TOIE2	:	SET_BIT( TIMSK , TOIE2_BIT);
						break;
		case OCIE2	:	SET_BIT( TIMSK , OCIE2_BIT);
						break;
		default		:	Local_enuErrorState = ES_OUT_RANGE;
						#warning " Timer_enuInterruptEnable() : Unidentified Interrupt Name, No Action Taken."
	}
	_SEI_ ;																// Enable Global Interrupts

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}

ES_t Timer_enuInterruptDisable( u8 Copy_u8TimerIntName)
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8SREG = SREG ;
	_CLI_ ;																// Disable ALL Interrupts

	switch ( Copy_u8TimerIntName )
	{
		case TOIE0	:	CLR_BIT( TIMSK , TOIE0_BIT);
						break;
		case OCIE0	:	CLR_BIT( TIMSK , OCIE0_BIT);
						break;
		case TOIE2	:	CLR_BIT( TIMSK , TOIE2_BIT);
						break;
		case OCIE2	:	CLR_BIT( TIMSK , OCIE2_BIT);
						break;
		default		:	Local_enuErrorState = ES_OUT_RANGE;
						#warning " Timer_enuInterruptEnable() : Unidentified Interrupt Name, No Action Taken."
	}

	SREG = Local_u8SREG ;												// Re-Setting Global Interrupts

	return ( (Local_enuErrorState == ES_NOK)? ES_OK : Local_enuErrorState ) ;
}
#if Mask
bool Timer_IsInterruptEnabled( u8 Copy_u8TimerIntName )
{
	u8 Local_u8Status = 0 ;

	switch ( Copy_u8TimerIntName )
	{
		case TOIE0	:	Local_u8Status = IS_BIT(TIMSK , TOIE0_BIT) ;
						break;
		case OCIE0	:	Local_u8Status = IS_BIT(TIMSK , OCIE0_BIT) ;
						break;
		case TOIE2	:	Local_u8Status = IS_BIT(TIMSK , TOIE2_BIT) ;
						break;
		case OCIE2	:	Local_u8Status = IS_BIT(TIMSK , OCIE2_BIT) ;
						break;
	}
	return ( ( Local_u8Status)? TRUE : FALSE ) ;
}
#endif

ES_t Timer_enuCallBack( u8 Copy_u8TimerIntName , void (*Copy_pAppFun)(void) )
{
	ES_t Local_enuErrorState = ES_OUT_RANGE;

	if( Copy_pAppFun != NULL )
	{
		for( u8 Local_u8Iter; Local_u8Iter < TIMERS_INTERRUPTS ; Local_u8Iter++ )
		{
			if( Global_AstrTimerPointers[Local_u8Iter].InterruptName == Copy_u8TimerIntName )
			{
				Global_AstrTimerPointers[Local_u8Iter].ptrFun = Copy_pAppFun;
				Local_enuErrorState = ES_OK ;
				break;
			}
		}
	}
	else Local_enuErrorState = ES_NULL_POINTER;

	return Local_enuErrorState ;
}

void __vector_4( void )__attribute__((signal));		/*	Timer2	Compare Match Interrupt	ISR		*/
void __vector_5( void )__attribute__((signal));		/*	Timer2	Overflow Interrupt	ISR			*/
void __vector_10( void )__attribute__((signal));	/*	Timer0	Compare Match Interrupt	ISR		*/
void __vector_11( void )__attribute__((signal));	/*	Timer0	Overflow Interrupt	ISR			*/

void __vector_4( void )/*	OCIE2	*/
{
	if( Global_AstrTimerPointers[3].ptrFun != NULL )
	{
		(*Global_AstrTimerPointers[3].ptrFun)( );
	}
}

void __vector_5( void )/*	TOIE2	*/
{
	if( Global_AstrTimerPointers[2].ptrFun != NULL )
	{
		(*Global_AstrTimerPointers[2].ptrFun)( );
	}
}
void __vector_10( void )/*	OCIE0	*/
{
	if( Global_AstrTimerPointers[1].ptrFun != NULL )
	{
		(*Global_AstrTimerPointers[1].ptrFun)( );
	}
}
void __vector_11( void )/*	TOIE0	*/
{
	if( Global_AstrTimerPointers[0].ptrFun != NULL )
	{
		(*Global_AstrTimerPointers[0].ptrFun)( );
	}
}




