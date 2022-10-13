/*
 * SevSeg1_prog.c
 *
 *  Created on: Aug 9, 2022
 *      Author: basse
 */

#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"
#include "..\..\SHARED\BIT_MATH.h"

#include "..\..\MCAL\DIO/DIO_int.h"

#include "SevSeg_priv.h"
#include "SevSeg_config.h"

extern u8 SevSeg_u8MaxModules;
extern u8 ModuleValuePins;
extern SSegModule_t SSegModule[];

ES_t SevSeg_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK;

	for( u8 Local_u8Counter = 0 ; Local_u8Counter <= SevSeg_u8MaxModules ; Local_u8Counter++)
	{
		for( u8 Local_u8PinNum = 0 ; Local_u8PinNum < ModuleValuePins ; Local_u8PinNum++ )
		{
			if( 	ES_OK == DIO_enuSetPinDirection(	SSegModule[Local_u8Counter].Value_Conf[Local_u8PinNum].InputGrp ,
														SSegModule[Local_u8Counter].Value_Conf[Local_u8PinNum].InputPin , DIO_u8OUTPUT) )
			Local_enuErrorState = DIO_enuSetPinValue(	SSegModule[Local_u8Counter].Value_Conf[Local_u8PinNum].InputGrp ,
														SSegModule[Local_u8Counter].Value_Conf[Local_u8PinNum].InputPin , SEGMENT_OFF );
			if( Local_enuErrorState != ES_OK) break;
		}
		if( Local_enuErrorState == ES_OK)
		{
			if( 	ES_OK == DIO_enuSetPinDirection(	SSegModule[Local_u8Counter].DIP_Conf.InputGrp ,
														SSegModule[Local_u8Counter].DIP_Conf.InputPin , DIO_u8OUTPUT) )
			Local_enuErrorState = DIO_enuSetPinValue(	SSegModule[Local_u8Counter].DIP_Conf.InputGrp ,
														SSegModule[Local_u8Counter].DIP_Conf.InputPin , DIP_OFF );
		}
		if( Local_enuErrorState == ES_OK)
		{
			if( 	ES_OK == DIO_enuSetPinDirection(	SSegModule[Local_u8Counter].Enable_Conf.InputGrp ,
														SSegModule[Local_u8Counter].Enable_Conf.InputPin , DIO_u8OUTPUT) )
			Local_enuErrorState = DIO_enuSetPinValue(	SSegModule[Local_u8Counter].Enable_Conf.InputGrp ,
														SSegModule[Local_u8Counter].Enable_Conf.InputPin , MOD_EN_OFF );
		}
		if( Local_enuErrorState != ES_OK) break;
	}

	return Local_enuErrorState ;//DONE
}
ES_t SevSeg_enuSetDigitValue(u8 Copy_u8ModuleNum , u8 Copy_u8SevSegDigitValue)
{
	ES_t Local_enuErrorState=ES_NOK ;
	
	if( ( (Copy_u8ModuleNum - MODULE_1) < SevSeg_u8MaxModules ) &&	( Copy_u8SevSegDigitValue < DIGIT_BASE ) )
	{
		Copy_u8ModuleNum -= (u8)MODULE_1;
		u8 Local_u8BitValue;
		for( u8 Local_u8Counter = 0 ; Local_u8Counter < ModuleValuePins ; Local_u8Counter++ )
		{
			Local_u8BitValue = (((Copy_u8SevSegDigitValue >> Local_u8Counter ) & SevSeg_u8BIT_MASK )? SEGMENT_ON : SEGMENT_OFF );
			Local_enuErrorState = DIO_enuSetPinValue(	SSegModule[Copy_u8ModuleNum].Value_Conf[Local_u8Counter].InputGrp ,
														SSegModule[Copy_u8ModuleNum].Value_Conf[Local_u8Counter].InputPin ,
														Local_u8BitValue );
			if( Local_enuErrorState != ES_OK) break;
		}
	}
	else Local_enuErrorState = ES_OUT_RANGE;


	return Local_enuErrorState ;//DONE
}

ES_t SevSeg_enuSetDIPValue(u8 Copy_u8ModuleNum ,u8 Copy_u8SevSegDIPValue)
{
	ES_t Local_enuErrorState=ES_NOK;

	if( ( (Copy_u8ModuleNum - MODULE_1) < SevSeg_u8MaxModules ) &&
		( Copy_u8SevSegDIPValue == DIP_ON ||Copy_u8SevSegDIPValue == DIP_OFF ) )
	{
		Copy_u8ModuleNum -= (u8)MODULE_1;
		Local_enuErrorState = DIO_enuSetPinValue(	SSegModule[Copy_u8ModuleNum].DIP_Conf.InputGrp ,
													SSegModule[Copy_u8ModuleNum].DIP_Conf.InputPin , Copy_u8SevSegDIPValue );
	}
	else Local_enuErrorState = ES_OUT_RANGE;

	return Local_enuErrorState ;//DONE
}

ES_t SevSeg_enuFrameDelay(u8 Copy_u8SevSegTotalModules, u8 *Copy_pu8SevSegModuleDelay)
{
	ES_t Local_enuErrorState=ES_NOK;

	if(Copy_u8SevSegTotalModules <= SevSeg_u8MaxModules)
	{
		*Copy_pu8SevSegModuleDelay = SevSeg_u8MODULE_DELAY( Copy_u8SevSegTotalModules );

		Local_enuErrorState=ES_OK;
	}
	else Local_enuErrorState = ES_OUT_RANGE;

	return Local_enuErrorState ;//DONE
}

ES_t SevSeg_enuModuleControl(u8 Copy_u8ModuleNum , u8 Copy_u8ModuleStatus)
{
	ES_t Local_enuErrorState=ES_NOK;

	if( ( Copy_u8ModuleStatus == MODULE_DISABLE || Copy_u8ModuleStatus == MODULE_ENABLE ) &&
		( Copy_u8ModuleNum - MODULE_1 < SevSeg_u8MaxModules ) )
	{
		Copy_u8ModuleNum -= (u8)MODULE_1;
		u8 Local_u8ModuleStatus = ( Copy_u8ModuleStatus == MODULE_ENABLE ? MOD_EN_ON : MOD_EN_OFF );
		Local_enuErrorState = DIO_enuSetPinValue(	SSegModule[Copy_u8ModuleNum].Enable_Conf.InputGrp ,
													SSegModule[Copy_u8ModuleNum].Enable_Conf.InputPin ,
													Local_u8ModuleStatus );
	}
	else	Local_enuErrorState = ES_OUT_RANGE;

	return Local_enuErrorState ;//DONE
}

