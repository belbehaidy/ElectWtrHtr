/*
 * LM35_int.h
 *
 *  Created on: Aug 21, 2022
 *      Author: basse
 */

#ifndef HAL_LM35_LM35_INT_H_
#define HAL_LM35_LM35_INT_H_

#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"

#include "..\..\MCAL\DIO\DIO_int.h"
#include "..\..\MCAL\ADC\ADC_int.h"


void ADC_vidISR(void);

bool Global_blConverted = FALSE ;

#define TEMP_VALUE_GRP			DIO_u8GROUP_A
#define TEMP_VALUE_PIN			DIO_u8PIN0
#define TEMP_VALUE_STATE		DIO_u8FLOAT

#define TEMP_ADC_CH				CH_00
#define TEMP_CONVERSION_FACTOR	0.488281 			//	Degree/step
#define TEMP_OFFSET				0.00002816			//	Correction factor at mid range

ES_t LM35_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK , Local_AenuErrorState[2];

	Local_AenuErrorState[0] = DIO_enuSetPinDirection( TEMP_VALUE_GRP , TEMP_VALUE_PIN , DIO_u8INPUT );
	Local_AenuErrorState[1] = DIO_enuSetPinValue( TEMP_VALUE_GRP , TEMP_VALUE_PIN , TEMP_VALUE_STATE );

	if( Local_AenuErrorState[0] == ES_OK && Local_AenuErrorState[1] == ES_OK )
	{
		Local_AenuErrorState[0] = ADC_enuInit();
		Local_AenuErrorState[1] = ADC_enuSelectChannel( TEMP_ADC_CH );
 	}
	if( Local_AenuErrorState[0] == ES_OK && Local_AenuErrorState[1] == ES_OK )
	{

		Local_AenuErrorState[0] = ADC_enuCallBack( ADC_vidISR );
		Local_AenuErrorState[1] = ADC_enuStartConversion();
	}
	if( Local_AenuErrorState[0] == ES_OK && Local_AenuErrorState[1] == ES_OK )
		Local_enuErrorState = ES_OK ;

	return Local_enuErrorState ;
}

ES_t LM35_enuReadTemp( u16 *Copy_u16TempValue )
{
	ES_t Local_enuErrorState = ES_NOK ;
	u16 Local_u16TempValue;

	if( Global_blConverted == TRUE )
	{
		Local_enuErrorState = ADC_enuRead( &Local_u16TempValue );
		*Copy_u16TempValue = (u16)( ( Local_u16TempValue * TEMP_CONVERSION_FACTOR ) + TEMP_OFFSET );
		Global_blConverted = FALSE ;
		Local_enuErrorState = ES_OK ;
	}

	return Local_enuErrorState ;
}

void ADC_vidISR(void)
{
	Global_blConverted = TRUE;
}

#endif /* HAL_LM35_LM35_INT_H_ */
