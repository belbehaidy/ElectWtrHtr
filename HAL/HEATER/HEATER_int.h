/*
 * HEATER_int.h
 *
 *  Created on: Oct 9, 2022
 *      Author: basse
 */

#ifndef HAL_HEATER_HEATER_INT_H_
#define HAL_HEATER_HEATER_INT_H_

#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"

#include "..\..\MCAL\DIO\DIO_int.h"
#include "..\..\MCAL\PWM\PWM_int.h"

#define HEATER_GRP			DIO_u8GROUP_D
#define HEATER_PIN			DIO_u8PIN4
#define HEATER_PWM			TIMER1B

#define HTR_TEMP_TOLERANCE 		5

#define HTR_KP					8
#define HTR_KI					4
#define HTR_KD					2

#ifndef ICR1_VALUE
#define ICR1_VALUE				0x00FF
#endif

#define HTR_INIT_DUTY_CYCLE		50
#define HTR_MAX_DUTY_CYCLE		80

ES_t Heater_enuInit(void)
{
	ES_t Local_AenuErrorState[5];

	Local_AenuErrorState[0] = PWM_enuInit();
	Local_AenuErrorState[1] = PWM_enuSetICR1Value( (u16)ICR1_VALUE );
	Local_AenuErrorState[2] = PWM_enuSetDutyCycle( HEATER_PWM , HTR_INIT_DUTY_CYCLE );
	Local_AenuErrorState[3] = DIO_enuSetPinDirection( HEATER_GRP , HEATER_PIN , DIO_u8OUTPUT );
	Local_AenuErrorState[4] = DIO_enuSetPinValue( HEATER_GRP , HEATER_PIN , DIO_u8LOW );

	u8 Local_u8Iter = 0 ;
	for( ; (Local_u8Iter < 5) && ( Local_AenuErrorState[Local_u8Iter] == ES_OK ) ; Local_u8Iter++ );

	return ( ( Local_u8Iter == 5 )? ES_OK : Local_AenuErrorState[Local_u8Iter] );
}

ES_t Heater_enuSetState( s8 Copy_u8TempError )
{
	ES_t Local_enuErrorState = ES_NOK ;
	static u8 PrevTempError = 0 ;
	f32 DutyCycle;

	if( Copy_u8TempError >= HTR_TEMP_TOLERANCE )	DutyCycle = 0.0 ;
	else if( Copy_u8TempError < -HTR_TEMP_TOLERANCE )	DutyCycle = HTR_MAX_DUTY_CYCLE ;
	else
	{
		DutyCycle = ( ( HTR_KP * Copy_u8TempError ) +
					( HTR_KI * ( Copy_u8TempError + PrevTempError ) ) +
					( HTR_KD * ( Copy_u8TempError - PrevTempError ) ) ) ;
	}

	Local_enuErrorState = PWM_enuSetDutyCycle( HEATER_PWM , DutyCycle );

	PrevTempError = Copy_u8TempError ;

	return Local_enuErrorState ;
}

#endif /* HAL_HEATER_HEATER_INT_H_ */
