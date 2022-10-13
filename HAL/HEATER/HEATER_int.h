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

#define HTR_KP					0.8
#define HTR_KI					0.4
#define HTR_KD					0.2

#define HTR_INIT_DUTY_CYCLE		50
#define HTR_MAX_DUTY_CYCLE		80

ES_t Heater_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK ;

	if( ES_OK == PWM_enuInit())
	{
		PWM_enuSetDutyCycle( HEATER_PWM , HTR_INIT_DUTY_CYCLE );
		if( ES_OK == DIO_enuSetPinDirection( HEATER_GRP , HEATER_PIN , DIO_u8OUTPUT ) )
			Local_enuErrorState = DIO_enuSetPinValue( HEATER_GRP , HEATER_PIN , DIO_u8LOW );
	}

	return Local_enuErrorState ;
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
