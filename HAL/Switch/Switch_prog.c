/*
 * Sw_One_prog.c
 *
 *  Created on: Aug 6, 2022
 *      Author: Ahmed El-Gaafrawy
 */
#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"

#include "..\..\MCAL\DIO\DIO_int.h"

#include "Switch_config.h"
#include "Switch_priv.h"

extern u8 Switch_u8SwitchesMaxNum ;
extern Switch_t Switch[];


ES_t Switch_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK , Local_enuAerror[2];

	for(u8 Local_u8Iter;Local_u8Iter < Switch_u8SwitchesMaxNum ;Local_u8Iter++)
	{
		Local_enuAerror[0] = DIO_enuSetPinDirection	(Switch[Local_u8Iter].sw_Grp , Switch[Local_u8Iter].sw_Pin , DIO_u8INPUT);
		Local_enuAerror[1] = DIO_enuSetPinValue		(Switch[Local_u8Iter].sw_Grp , Switch[Local_u8Iter].sw_Pin , Switch[Local_u8Iter].sw_State);

		if (Local_enuAerror[0] != ES_OK || Local_enuAerror[1] != ES_OK) break;
	}

	if (Local_enuAerror[0] == ES_OK && Local_enuAerror[1] == ES_OK)
		Local_enuErrorState = ES_OK;

	return Local_enuErrorState ;
}

ES_t Switch_enuGetPressed (u8 Copy_u8SwitchNum ,u8 *Copy_pu8SwitchValue)
{
	ES_t Local_enuErrorState = ES_NOK;


	if( (Copy_u8SwitchNum -= SWITCH_ZERO) < Switch_u8SwitchesMaxNum )
	{
		Local_enuErrorState =  DIO_enuGetPinValue(Switch[Copy_u8SwitchNum].sw_Grp , Switch[Copy_u8SwitchNum].sw_Pin , Copy_pu8SwitchValue ) ;
	}
	else Local_enuErrorState = ES_OUT_RANGE ;

	return Local_enuErrorState ;
}
