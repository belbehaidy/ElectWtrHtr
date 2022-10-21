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

#ifndef ICR1_VALUE
#define ICR1_VALUE				0x00FF
#endif

#define COOL_INIT_DUTY_CYCLE	50
#define COOL_MAX_DUTY_CYCLE		80


ES_t Coolent_enuInit(void)
{
	ES_t Local_AenuErrorState[5];

	Local_AenuErrorState[0] = PWM_enuInit();
	Local_AenuErrorState[1] = PWM_enuSetICR1Value( (u16)ICR1_VALUE );
	Local_AenuErrorState[2] = PWM_enuSetDutyCycle( COOLENT_PWM , COOL_INIT_DUTY_CYCLE );
	Local_AenuErrorState[3] = DIO_enuSetPinDirection( COOLENT_GRP , COOLENT_PIN , DIO_u8OUTPUT );
	Local_AenuErrorState[4] = DIO_enuSetPinValue( COOLENT_GRP , COOLENT_PIN , DIO_u8LOW );

	u8 Local_u8Iter = 0 ;
	for( ; (Local_u8Iter < 5) && ( Local_AenuErrorState[Local_u8Iter] == ES_OK ) ; Local_u8Iter++ );

	return ( ( Local_u8Iter == 5 )? ES_OK : Local_AenuErrorState[Local_u8Iter] );
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
