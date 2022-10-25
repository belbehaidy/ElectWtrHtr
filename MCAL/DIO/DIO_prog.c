/*
 * DIO_prog.c
 *
 *  Created on: Oct 6, 2022
 *      Author: Bassem El_Behaidy
 */

#include "../../SHARED/stdTypes.h"
#include "../../SHARED/errorState.h"
#include "../../SHARED/BIT_MATH.h"
#include "../../SHARED/ATMEGA32_Registers.h"


#include "DIO_priv.h"


ES_t DIO_enuInit(void)
{
	return ES_OK ;
}

ES_t DIO_enuSetPinDirection(u8 Copy_u8GroupID , u8 Copy_u8PinID , u8 Copy_u8Direction)
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_u8GroupID <= DIO_u8GROUP_D &&
			Copy_u8PinID <= DIO_u8PIN7 &&
			Copy_u8Direction <= DIO_u8OUTPUT )
	{
		switch (Copy_u8GroupID)
		{
		case DIO_u8GROUP_A:
			ASSIGN_BIT_VALUE( DDR_A , Copy_u8PinID , Copy_u8Direction );
			break;
		case DIO_u8GROUP_B:
			ASSIGN_BIT_VALUE( DDR_B , Copy_u8PinID , Copy_u8Direction );
			break;
		case DIO_u8GROUP_C:
			ASSIGN_BIT_VALUE( DDR_C , Copy_u8PinID , Copy_u8Direction );
			break;
		case DIO_u8GROUP_D:
			ASSIGN_BIT_VALUE( DDR_D , Copy_u8PinID , Copy_u8Direction );
			break;
		}
		Local_enuErrorState = ES_OK;
	}
	else
	{
		Local_enuErrorState = ES_OUT_RANGE;
	}

	return Local_enuErrorState ;
}

ES_t DIO_enuSetPinValue(u8 Copy_u8GroupID , u8 Copy_u8PinID , u8 Copy_u8Value)
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_u8GroupID <= DIO_u8GROUP_D &&
			Copy_u8PinID <= DIO_u8PIN7 &&
			(Copy_u8Value <= DIO_u8HIGH || Copy_u8Value <= DIO_u8PULL_UP ) )
	{
		switch (Copy_u8GroupID)
		{
		case DIO_u8GROUP_A:
			ASSIGN_BIT_VALUE( PORT_A , Copy_u8PinID , Copy_u8Value );

			break;
		case DIO_u8GROUP_B:
			ASSIGN_BIT_VALUE( PORT_B , Copy_u8PinID , Copy_u8Value );

			break;
		case DIO_u8GROUP_C:
			ASSIGN_BIT_VALUE( PORT_C , Copy_u8PinID , Copy_u8Value );
			break;
		case DIO_u8GROUP_D:
			ASSIGN_BIT_VALUE( PORT_D , Copy_u8PinID , Copy_u8Value );
			break;
		}
		Local_enuErrorState = ES_OK;
	}
	else
	{
		Local_enuErrorState = ES_OUT_RANGE;
	}

	return Local_enuErrorState ;
}


ES_t DIO_enuTogglePinValue(u8 Copy_u8GroupID , u8 Copy_u8PinID)
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_u8GroupID <= DIO_u8GROUP_D &&
			Copy_u8PinID <= DIO_u8PIN7)
	{
		switch (Copy_u8GroupID)
		{
		case DIO_u8GROUP_A:
			TOG_BIT( PORT_A , Copy_u8PinID );
			break;
		case DIO_u8GROUP_B:
			TOG_BIT( PORT_A , Copy_u8PinID );
			break;
		case DIO_u8GROUP_C:
			TOG_BIT( PORT_A , Copy_u8PinID );
			break;
		case DIO_u8GROUP_D:
			TOG_BIT( PORT_A , Copy_u8PinID );
			break;
		}
		Local_enuErrorState = ES_OK;
	}
	else
	{
		Local_enuErrorState = ES_OUT_RANGE;
	}

	return Local_enuErrorState ;
}

ES_t DIO_enuGetPinValue(u8 Copy_u8GroupID , u8 Copy_u8PinID , u8 *Copy_pu8Value)
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_pu8Value != NULL)
	{
		if (Copy_u8GroupID <= DIO_u8GROUP_D &&
				Copy_u8PinID <= DIO_u8PIN7)
		{
			switch (Copy_u8GroupID)
			{
			case DIO_u8GROUP_A:
				*Copy_pu8Value = IS_BIT( PIN_A , Copy_u8PinID ) ;
				break;
			case DIO_u8GROUP_B:
				*Copy_pu8Value = IS_BIT( PIN_B , Copy_u8PinID ) ;
				break;
			case DIO_u8GROUP_C:
				*Copy_pu8Value = IS_BIT( PIN_C , Copy_u8PinID ) ;
				break;
			case DIO_u8GROUP_D:
				*Copy_pu8Value = IS_BIT( PIN_D , Copy_u8PinID ) ;
				break;
			}
			Local_enuErrorState = ES_OK;
		}
		else
		{
			Local_enuErrorState = ES_OUT_RANGE;
		}
	}
	else
	{
		Local_enuErrorState = ES_NULL_POINTER;
	}

	return Local_enuErrorState ;
}

