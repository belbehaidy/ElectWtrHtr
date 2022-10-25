/*
 * Switch_config.c

 *
 *  Created on: Aug 14, 2022
 *      Author: Bassem El-Behaidy
 */

#include "..\..\SHARED\ATMEGA32_Registers.h"
#include "..\..\SHARED\BIT_MATH.h"
#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"


#include "..\..\MCAL\DIO\DIO_int.h"

#include "Switch_config.h"
#include "Switch_priv.h"


u8 Switch_u8SwitchesMaxNum = SWITCHES_MAX_NUM ;

Switch_t Switch[SWITCHES_MAX_NUM] = {
											{	SW_ZERO_u8GROUP	, SW_ZERO_u8PIN	, SW_ZERO_u8STATE	},
											{	SW_ONE_u8GROUP	, SW_ONE_u8PIN	, SW_ONE_u8STATE	},
											{	SW_TWO_u8GROUP	, SW_TWO_u8PIN	, SW_TWO_u8STATE	}
									};
