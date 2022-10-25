/*
 * COOLENT_int.h
 *
 *  Created on: Oct 9, 2022
 *      Author: basse
 */

#ifndef HAL_COOLENT_COOLENT_INT_H_
#define HAL_COOLENT_COOLENT_INT_H_

#include "..\..\SHARED\ATMEGA32_Registers.h"
#include "..\..\SHARED\BIT_MATH.h"
#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"

#include "..\..\MCAL\DIO\DIO_int.h"
#include "..\..\MCAL\PWM\PWM_int.h"

#define COOLENT_GRP			DIO_u8GROUP_D
#define COOLENT_PORT		_SFR_PORT_D_
#define COOLENT_CNTRL		DIO_u8PIN5
#define COOLENT_EN			DIO_u8PIN6
#define COOLENT_PWM			TIMER1A

#define COOLENT_TEMP_TOLERANCE 		5

#define COOLENT_DUTY_CYC_TOL		0.0F

#define COOL_KP					5.0F
#define COOL_KI					2.0F
#define COOL_KD					2.0F

#ifndef ICR1_VALUE
#define COOLENT_PWM_SETUP
#define ICR1_VALUE				0x0270
#endif

#define COOL_INIT_DUTY_CYCLE	0.0F
#define COOL_MAX_DUTY_CYCLE		80.0F


ES_t Coolent_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK , Local_AenuErrorState[2];
	u8 Local_u8Iter = 0 ;

#ifdef COOLENT_PWM_SETUP
	Local_AenuErrorState[0] = PWM_enuInit();
	Local_AenuErrorState[1] = PWM_enuSetICR1Value( (u16)ICR1_VALUE );
	for( Local_u8Iter = 0 ; (Local_u8Iter < 2) && ( Local_AenuErrorState[Local_u8Iter] == ES_OK ) ; Local_u8Iter++ );
	if( Local_u8Iter == 2 )
	{
#endif
		Local_AenuErrorState[0] = DIO_enuSetPinDirection( COOLENT_GRP , COOLENT_CNTRL , DIO_u8OUTPUT );
		Local_AenuErrorState[1] = DIO_enuSetPinValue( COOLENT_GRP , COOLENT_CNTRL , DIO_u8LOW );

#ifdef COOLENT_PWM_SETUP
	}
#endif
	for( Local_u8Iter = 0 ; (Local_u8Iter < 2) && ( Local_AenuErrorState[Local_u8Iter] == ES_OK ) ; Local_u8Iter++ );
	if( Local_u8Iter == 2 )
	{
		Local_AenuErrorState[0] = DIO_enuSetPinDirection( COOLENT_GRP , COOLENT_EN , DIO_u8OUTPUT );
		Local_AenuErrorState[1] = DIO_enuSetPinValue( COOLENT_GRP , COOLENT_EN , DIO_u8LOW );
	}
	for( Local_u8Iter = 0 ; (Local_u8Iter < 2) && ( Local_AenuErrorState[Local_u8Iter] == ES_OK ) ; Local_u8Iter++ );
	if( Local_u8Iter == 2)
		Local_enuErrorState = PWM_enuSetDutyCycle( COOLENT_PWM , COOL_INIT_DUTY_CYCLE );


	return  Local_enuErrorState  ;
}

ES_t Coolent_enuEnable( void )
{

	ASM_SET_BIT( COOLENT_PORT , COOLENT_EN ) ;

	return ES_OK ;
}

ES_t Coolent_enuDisable( void )
{
	ASM_CLR_BIT( COOLENT_PORT , COOLENT_EN ) ;

	return ES_OK ;
}


ES_t Coolent_enuSetState( s8 Copy_s8TempError )
{
	ES_t Local_enuErrorState = ES_NOK ;
	static s8 PrevTempError = 0 , AccTempError = 0;
	f32 DutyCycle;
	static f32 PrevDutyCycle = 0.0F ;

	Copy_s8TempError = (-1)* Copy_s8TempError ;

	if( Copy_s8TempError >= COOLENT_TEMP_TOLERANCE )
		DutyCycle = 0.0 ;
	else if( Copy_s8TempError < 0 )
		DutyCycle = COOL_MAX_DUTY_CYCLE ;
	else
	{
		AccTempError += Copy_s8TempError ;
		DutyCycle = COOL_MAX_DUTY_CYCLE - (	( COOL_KP * Copy_s8TempError 	) +
											( COOL_KI * AccTempError 		) +
											( COOL_KD * ( Copy_s8TempError - PrevTempError ) ) ) ;
		if( DutyCycle < 0.0 ) DutyCycle = 0.0 ;
		else if( DutyCycle > COOL_MAX_DUTY_CYCLE ) DutyCycle = COOL_MAX_DUTY_CYCLE ;

	}

	if( ( DutyCycle >= ( PrevDutyCycle + COOLENT_DUTY_CYC_TOL ) )	|| ( DutyCycle = COOL_MAX_DUTY_CYCLE )	||
		( DutyCycle <= ( PrevDutyCycle - COOLENT_DUTY_CYC_TOL ) )	|| ( DutyCycle == 0.0F ) )
	{
		PrevDutyCycle = DutyCycle ;
		Local_enuErrorState = PWM_enuSetDutyCycle( COOLENT_PWM , DutyCycle );
	}

	PrevTempError = Copy_s8TempError ;

	return Local_enuErrorState ;
}

#endif /* HAL_COOLENT_COOLENT_INT_H_ */
