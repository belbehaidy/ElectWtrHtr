/*
 * SevSeg_config.c
 *
 *  Created on: Aug 14, 2022
 *      Author: Bassem El-Behaidy
 */

#include "..\..\SHARED\ATMEGA32_Registers.h"
#include "..\..\SHARED\BIT_MATH.h"
#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"

#include "..\..\MCAL\DIO\DIO_int.h"

#include "SevSeg_config.h"
#include "SevSeg_priv.h"


//u8 SevSeg_u8MaxModules = SEV_SEG_MAX_MODULES;
u8 ModuleValuePins = MODULE_VALUE_PINS;

SSegModule_t SSegModule =	{
								{
									{ SevSeg_u8_A_GROUP   , SevSeg_u8_A_PIN   }, 			/*		PIN A			*/
									{ SevSeg_u8_B_GROUP   , SevSeg_u8_B_PIN   },			/*		PIN B			*/
									{ SevSeg_u8_C_GROUP   , SevSeg_u8_C_PIN   },			/*		PIN C			*/
									{ SevSeg_u8_D_GROUP   , SevSeg_u8_D_PIN   }			/*		PIN D			*/
								},
									{ SevSeg_u8_DIP_GROUP , SevSeg_u8_DIP_PIN },			/*		PIN DIP			*/
									{ SevSeg_u8MOD_1_EN_GROUP  , SevSeg_u8MOD_1_EN_PIN 	},	/*	Module 1	PIN EN	*/
									{ SevSeg_u8MOD_2_EN_GROUP  ,SevSeg_u8MOD_2_EN_PIN   }	/*	Module 2	PIN EN	*/
							};

