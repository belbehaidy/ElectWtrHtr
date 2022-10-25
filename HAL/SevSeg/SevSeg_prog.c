/*
 * SevSeg1_prog.c
 *
 *  Created on: Aug 9, 2022
 *      Author: basse
 */

#include "../../SHARED/ATMEGA32_Registers.h"
#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"
#include "..\..\SHARED\BIT_MATH.h"

#include "..\..\MCAL\DIO/DIO_int.h"

#include "SevSeg_priv.h"
#include "SevSeg_config.h"

extern u8 ModuleValuePins;
extern SSegModule_t SSegModule;

ES_t SevSeg_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK;

	for( u8 Local_u8PinNum = 0 ; Local_u8PinNum < ModuleValuePins ; Local_u8PinNum++ )
	{
		if( 	ES_OK == DIO_enuSetPinDirection(	SSegModule.Value_Conf[Local_u8PinNum].InputGrp ,
													SSegModule.Value_Conf[Local_u8PinNum].InputPin , DIO_u8OUTPUT) )
		Local_enuErrorState = DIO_enuSetPinValue(	SSegModule.Value_Conf[Local_u8PinNum].InputGrp ,
													SSegModule.Value_Conf[Local_u8PinNum].InputPin , SEGMENT_OFF );
		if( Local_enuErrorState != ES_OK) break;
	}
	if( Local_enuErrorState == ES_OK)
	{
		if( 	ES_OK == DIO_enuSetPinDirection(	SSegModule.DIP_Conf.InputGrp ,
													SSegModule.DIP_Conf.InputPin , DIO_u8OUTPUT) )
		Local_enuErrorState = DIO_enuSetPinValue(	SSegModule.DIP_Conf.InputGrp ,
													SSegModule.DIP_Conf.InputPin , DIP_OFF );
	}
	if( Local_enuErrorState == ES_OK)
	{
		if( 	ES_OK == DIO_enuSetPinDirection(	SSegModule.Enable1_Conf.InputGrp ,
													SSegModule.Enable1_Conf.InputPin , DIO_u8OUTPUT) )
		Local_enuErrorState = DIO_enuSetPinValue(	SSegModule.Enable1_Conf.InputGrp ,
													SSegModule.Enable1_Conf.InputPin , MOD_EN_OFF );
	}
	if( Local_enuErrorState == ES_OK)
	{
		if( 	ES_OK == DIO_enuSetPinDirection(	SSegModule.Enable2_Conf.InputGrp ,
													SSegModule.Enable2_Conf.InputPin , DIO_u8OUTPUT) )
		Local_enuErrorState = DIO_enuSetPinValue(	SSegModule.Enable2_Conf.InputGrp ,
													SSegModule.Enable2_Conf.InputPin , MOD_EN_OFF );
	}

	return Local_enuErrorState ;//DONE
}
ES_t SevSeg_enuSetDigitValue( u8 Copy_u8SevSegDigitValue )
{
/*
	if( SEGMENT_ON == DIO_u8LOW )
		Copy_u8SevSegDigitValue = (~Copy_u8SevSegDigitValue) ;

	asm volatile(	" push r18" "\n\t"			\
					" push r20" "\n\t"			\
					" in r18 , %0" "\n\t"		\
					" and r18 , 0xF0" "\n\t"	\
					" LDI r20 , %1"	"\n\t"		\
					" and r20 , 0x0F" "\n\t"	\
					" or r20 , r18" "\n\t"		\
					" out %2 , r20" "\n\t"		\
					" pop r20" "\n\t"			\
					" pop r18" "\n\t"			\
					:							\
					:"M" ( _SFR_PIN_B_ ),		\
					 "r" ( &Copy_u8SevSegDigitValue ),\
					 "M" ( _SFR_PORT_B_ )		\
				);
*/
	PORT_B = ( (PORT_B = _u8_LOW_NIPPLE_VALUE_( PORT_B ) )) | ( _u8_LOW_NIPPLE_VALUE_( Copy_u8SevSegDigitValue ) << _NIPPLE_SHIFT_ ) ;

	return ES_OK ;//DONE
}

ES_t SevSeg_enuSetDIPValue(u8 Copy_u8SevSegDIPValue)
{
	ES_t Local_enuErrorState=ES_NOK;

	if( Copy_u8SevSegDIPValue == DIP_ON ||Copy_u8SevSegDIPValue == DIP_OFF )
	{
		Local_enuErrorState = DIO_enuSetPinValue(	SSegModule.DIP_Conf.InputGrp ,
													SSegModule.DIP_Conf.InputPin , Copy_u8SevSegDIPValue );
	}
	else Local_enuErrorState = ES_OUT_RANGE;

	return Local_enuErrorState ;//DONE
}

ES_t SevSeg_enuFrameDelay(u8 Copy_u8SevSegTotalModules, u8 *Copy_pu8SevSegModuleDelay)
{
	ES_t Local_enuErrorState=ES_NOK;

	*Copy_pu8SevSegModuleDelay = SevSeg_u8MODULE_DELAY( Copy_u8SevSegTotalModules );

	Local_enuErrorState=ES_OK;

	return Local_enuErrorState ;//DONE
}

ES_t SevSeg_enuModuleEnable(u8 Copy_u8ModuleNum )
{
	ES_t Local_enuErrorState=ES_NOK;

	if( Copy_u8ModuleNum == MODULE_1)
		Local_enuErrorState = DIO_enuSetPinValue(	SSegModule.Enable1_Conf.InputGrp ,
													SSegModule.Enable1_Conf.InputPin ,	MOD_EN_ON );
	else if( Copy_u8ModuleNum == MODULE_2)
		Local_enuErrorState = DIO_enuSetPinValue(	SSegModule.Enable2_Conf.InputGrp ,
																	SSegModule.Enable2_Conf.InputPin ,	MOD_EN_ON );
	else Local_enuErrorState = ES_OUT_RANGE ;

	return Local_enuErrorState ;//DONE
}

ES_t SevSeg_enuModuleDisable(u8 Copy_u8ModuleNum )
{
	ES_t Local_enuErrorState=ES_NOK;

	if( Copy_u8ModuleNum == MODULE_1)
		Local_enuErrorState = DIO_enuSetPinValue(	SSegModule.Enable1_Conf.InputGrp ,
													SSegModule.Enable1_Conf.InputPin ,	MOD_EN_OFF );
	else if( Copy_u8ModuleNum == MODULE_2)
		Local_enuErrorState = DIO_enuSetPinValue(	SSegModule.Enable2_Conf.InputGrp ,
																	SSegModule.Enable2_Conf.InputPin ,	MOD_EN_OFF );
	else Local_enuErrorState = ES_OUT_RANGE ;

	return Local_enuErrorState ;//DONE
}
