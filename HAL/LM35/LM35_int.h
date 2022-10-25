/*
 * LM35_int.h
 *
 *  Created on: Aug 21, 2022
 *      Author: basse
 */

#ifndef HAL_LM35_LM35_INT_H_
#define HAL_LM35_LM35_INT_H_

#include "..\..\SHARED\ATMEGA32_Registers.h"
#include "..\..\SHARED\BIT_MATH.h"
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

/********************************************************************************************/
#define TEMP_CONVERSION_FACTOR	1.941216926					/*	Average Conversion Factor	*/
#define TEMP_OFFSET				(1.040630271)				/*	Average Correction Factor	*/
/*	Above Values for Both Conversion Factor & Correction Factor result in 					*/
/*	Temperature accuracy of (+/- 1 Degree Celsius ) along the range from 20 - 100 Degrees	*/
/********************************************************************************************/


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

ES_t LM35_enuReadTemp( u8 *Copy_u8TempValue )
{
	ES_t Local_enuErrorState = ES_NOK ;
	u8 Local_u8TempValue;

	Global_blConverted = FALSE ;
	Local_enuErrorState = ADC_enuReadHigh( &Local_u8TempValue );
	*Copy_u8TempValue = (u8)( ( (f32)Local_u8TempValue * TEMP_CONVERSION_FACTOR ) + TEMP_OFFSET );

	return Local_enuErrorState ;
}

void ADC_vidISR(void)
{
	Global_blConverted = TRUE;
}

#endif /* HAL_LM35_LM35_INT_H_ */
