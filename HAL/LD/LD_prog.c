/*
 * LD_prog.c
 *
 *  Created on: Aug 6, 2022
 *      Author: basse
 */

#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"

#include "..\..\MCAL\DIO\DIO_int.h"

#include "LD_config.h"
#include "LD_priv.h"

extern u8 LD_u8LD_MaxNum;
extern LD_t LD[];


ES_t LD_enuInit(void)
{
	ES_t Local_enuErrorStatus = ES_NOK , Local_AenuStatus[2];

	for(u8 Local_u8Iter = 0; Local_u8Iter < LD_u8LD_MaxNum ; Local_u8Iter++)
	{
		Local_AenuStatus[0] = DIO_enuSetPinDirection( LD[Local_u8Iter].LD_Grp , LD[Local_u8Iter].LD_Pin , DIO_u8OUTPUT);
		Local_AenuStatus[1] = DIO_enuSetPinValue( LD[Local_u8Iter].LD_Grp , LD[Local_u8Iter].LD_Pin , LD_u8OFF);

		if(Local_AenuStatus[0] != ES_OK || Local_AenuStatus[1] != ES_OK) break;
	}

	if(Local_AenuStatus[0] == ES_OK && Local_AenuStatus[1] == ES_OK)
		Local_enuErrorStatus = ES_OK;

	return Local_enuErrorStatus;
}

ES_t LD_enuSetState(u8 Copy_u8LD_Num ,u8 Copy_u8LDSetValue)
{
	ES_t Local_enuErrorStatus = ES_NOK ;

	if( ( ( Copy_u8LD_Num -= LD_ZERO ) < LD_u8LD_MaxNum ) &&
		( Copy_u8LDSetValue == LD_u8ON || Copy_u8LDSetValue == LD_u8OFF ) )
	{
		Local_enuErrorStatus = DIO_enuSetPinValue(LD[Copy_u8LD_Num].LD_Grp , LD[Copy_u8LD_Num].LD_Pin , Copy_u8LDSetValue);
	}
	else Local_enuErrorStatus = ES_OUT_RANGE;


	return Local_enuErrorStatus;
}
