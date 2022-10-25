/*
 * EXTI_config.c
 *
 *  Created on: Aug 19, 2022
 *      Author: Bassem EL-Behaidy
 */

#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"
#include "..\..\SHARED\BIT_MATH.h"
#include "..\..\SHARED\ATMEGA32_Registers.h"

#include "EXTI_config.h"
#include "EXTI_priv.h"

u8 EXTI_u8EXTI_Num	=	EXTI_MAX_NUM;

EXTI_t EXTI_AstrEXTI_Config[EXTI_MAX_NUM] =
											{
													{ INT0	,INT0_SEN_LVL	, INT0_STATUS },
													{ INT1	,INT1_SEN_LVL	, INT1_STATUS },
													{ INT2	,INT2_SEN_LVL	, INT2_STATUS }
											};
