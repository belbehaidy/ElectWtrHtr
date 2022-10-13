/*
 * COOLENT_int.h
 *
 *  Created on: Oct 9, 2022
 *      Author: basse
 */

#ifndef HAL_COOLENT_COOLENT_INT_H_
#define HAL_COOLENT_COOLENT_INT_H_

#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"

#include "..\..\MCAL\DIO\DIO_int.h"
#include "..\..\MCAL\PWM\PWM_int.h"

#define COOLENT_GRP			DIO_u8GROUP_D
#define COOLENT_PIN			DIO_u8PIN5
#define COOLENT_PWM			TIMER1A

#define COOLENT_TEMP_TOLERANCE 		5

#define COOL_KP					8
#define COOL_KI					4
#define COOL_KD					2

#define COOL_INIT_DUTY_CYCLE	50
#define COOL_MAX_DUTY_CYCLE		80


ES_t Coolent_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK ;

	if( ES_OK == PWM_enuInit())
	{
		PWM_enuSetDutyCycle( COOLENT_PWM , COOL_INIT_DUTY_CYCLE );
		if( ES_OK == DIO_enuSetPinDirection( COOLENT_GRP , COOLENT_PIN , DIO_u8OUTPUT ) )
			Local_enuErrorState = DIO_enuSetPinValue( COOLENT_GRP , COOLENT_PIN , DIO_u8LOW );
	}
	return Local_enuErrorState ;
}

ES_t Coolent_enuSetState( s8 Copy_u8TempError )
{
	ES_t Local_enuErrorState = ES_NOK ;
	static u8 PrevTempError = 0 ;
	f32 DutyCycle;

	if( Copy_u8TempError <= -COOLENT_TEMP_TOLERANCE )	DutyCycle = 0.0 ;
	else if( Copy_u8TempError > COOLENT_TEMP_TOLERANCE )	DutyCycle = COOL_MAX_DUTY_CYCLE ;
	else
	{
		DutyCycle = ( ( COOL_KP * Copy_u8TempError ) +
					( COOL_KI * ( Copy_u8TempError + PrevTempError ) ) +
					( COOL_KD * ( Copy_u8TempError - PrevTempError ) ) ) ;
	}

	Local_enuErrorState = PWM_enuSetDutyCycle( COOLENT_PWM , DutyCycle );

	PrevTempError = Copy_u8TempError ;

	return Local_enuErrorState ;
}

#endif /* HAL_COOLENT_COOLENT_INT_H_ */
