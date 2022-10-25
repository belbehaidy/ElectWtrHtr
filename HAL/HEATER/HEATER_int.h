/*
 * HEATER_int.h
 *
 *  Created on: Oct 9, 2022
 *      Author: basse
 */

#ifndef HAL_HEATER_HEATER_INT_H_
#define HAL_HEATER_HEATER_INT_H_

#include "..\..\SHARED\ATMEGA32_Registers.h"
#include "..\..\SHARED\BIT_MATH.h"
#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"


#include "..\..\MCAL\DIO\DIO_int.h"
#include "..\..\MCAL\EXTI\EXTI_int.h"
#include "..\..\MCAL\PWM\PWM_int.h"

#define HEATER_GRP			DIO_u8GROUP_D
#define HEATER_PORT			_SFR_PORT_D_
#define HEATER_CNTRL		DIO_u8PIN4
#define HEATER_EN			DIO_u8PIN3
#define HEATER_PWM			TIMER1B

#define HTR_TEMP_TOLERANCE 		5

#define HTR_DUTY_CYC_TOL		0.0F

#define HTR_KP					5.0F
#define HTR_KI					2.0F
#define HTR_KD					2.0F

#ifndef ICR1_VALUE
#define HEATER_PWM_SETUP
#define ICR1_VALUE				0x0270
#endif

#define HTR_INIT_DUTY_CYCLE		0.0F
#define HTR_MAX_DUTY_CYCLE		80.0F

bool CounterReset = FALSE;
void INT0_ISR( void);
void Heater_vidZCrossSync( void );

ES_t Heater_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK , Local_AenuErrorState[2];
	u8 Local_u8Iter = 0 ;

#ifdef HEATER_PWM_SETUP
//	Local_AenuErrorState[0] = PWM_enuInit();
//	Local_AenuErrorState[1] = PWM_enuSetICR1Value( (u16)ICR1_VALUE );
	for( Local_u8Iter = 0 ; (Local_u8Iter < 2) && ( Local_AenuErrorState[Local_u8Iter] == ES_OK ) ; Local_u8Iter++ );
	if( Local_u8Iter == 2 )
	{
#endif
		Local_AenuErrorState[0] = DIO_enuSetPinDirection( HEATER_GRP , HEATER_CNTRL , DIO_u8OUTPUT ) ;
		Local_AenuErrorState[1] = DIO_enuSetPinValue( HEATER_GRP , HEATER_CNTRL , DIO_u8LOW ) ;
#ifdef HEATER_PWM_SETUP
	}
#endif

//	Heater_vidZCrossSync();

	for( Local_u8Iter = 0 ; (Local_u8Iter < 2) && ( Local_AenuErrorState[Local_u8Iter] == ES_OK ) ; Local_u8Iter++ );
	if( Local_u8Iter == 2 )
	{
		Local_AenuErrorState[0] = PWM_enuSetDutyCycle( HEATER_PWM , HTR_INIT_DUTY_CYCLE );
		Local_AenuErrorState[1] = EXTI_enuCallBack( INT0 , INT0_ISR );
	}
	for( Local_u8Iter = 0 ; (Local_u8Iter < 2) && ( Local_AenuErrorState[Local_u8Iter] == ES_OK ) ; Local_u8Iter++ );
	if( Local_u8Iter == 2 )
	{
		Local_AenuErrorState[0] = DIO_enuSetPinDirection( HEATER_GRP , HEATER_EN , DIO_u8OUTPUT ) ;
		Local_AenuErrorState[1] = DIO_enuSetPinValue( HEATER_GRP , HEATER_EN , DIO_u8LOW ) ;
	}
	for( Local_u8Iter = 0 ; (Local_u8Iter < 2) && ( Local_AenuErrorState[Local_u8Iter] == ES_OK ) ; Local_u8Iter++ );
	if( Local_u8Iter == 2 )
		Local_enuErrorState = ES_OK ;

	return Local_enuErrorState ;
}
/*
void Heater_vidZCrossSync( void )
{
	u8 Local_u8SREG = SREG ;
	_SEI_;
	while( 	CounterReset != TRUE);
	_CLI_;
	EXTI_enuCallBack( INT0 , NULL );
	EXTI_enuDisableInterrupt( INT0 );
	CounterReset = FALSE ;
	SREG = Local_u8SREG ;
}
*/
ES_t Heater_enuEnable( void )
{
	ASM_SET_BIT( HEATER_PORT , HEATER_EN );

	return ES_OK  ;
}

ES_t Heater_enuDisable( void )
{

	ASM_CLR_BIT( HEATER_PORT , HEATER_EN );

	return ES_OK ;
}


ES_t Heater_enuSetState( s8 Copy_s8TempError )
{
	ES_t Local_enuErrorState = ES_NOK ;
	static s8 PrevTempError = 0 , AccTempError = 0 ;
	static f32 PrevDutyCycle = 0.0F ;

	f32 DutyCycle;

	if( Copy_s8TempError >= HTR_TEMP_TOLERANCE )
		DutyCycle = 0.0F ;
	else if( Copy_s8TempError < 0 )
		DutyCycle = HTR_MAX_DUTY_CYCLE ;
	else
	{
		AccTempError += Copy_s8TempError ;
		DutyCycle =	HTR_MAX_DUTY_CYCLE - (	( HTR_KP * Copy_s8TempError	) +
											( HTR_KI * AccTempError		) +
											( HTR_KD * ( Copy_s8TempError - PrevTempError ) ) ) ;
		if( DutyCycle < 0.0 ) DutyCycle = 0.0 ;
		else if( DutyCycle > HTR_MAX_DUTY_CYCLE ) DutyCycle = HTR_MAX_DUTY_CYCLE ;
	}

	if( ( DutyCycle >= ( PrevDutyCycle + HTR_DUTY_CYC_TOL ) )	|| ( DutyCycle == HTR_MAX_DUTY_CYCLE )	||
		( DutyCycle <= ( PrevDutyCycle - HTR_DUTY_CYC_TOL ) )	|| ( DutyCycle == 0.0F ) )
	{
		PrevDutyCycle = DutyCycle ;
		Local_enuErrorState = PWM_enuSetDutyCycle( HEATER_PWM , DutyCycle );
	}


	PrevTempError = Copy_s8TempError ;

	return Local_enuErrorState ;
}

void INT0_ISR( void)
{
	TCNT1 = 0x0000 ;
//	CounterReset = TRUE;
}

#endif /* HAL_HEATER_HEATER_INT_H_ */
