/*
 * SevSeg_config.c
 *
 *  Created on: Aug 14, 2022
 *      Author: Bassem El-Behaidy
 */

#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"

#include "..\..\MCAL\DIO\DIO_int.h"

#include "SevSeg_config.h"
#include "SevSeg_priv.h"


u8 SevSeg_u8MaxModules = SEV_SEG_MAX_MODULES;
u8 ModuleValuePins = MODULE_VALUE_PINS;

SSegModule_t SSegModule[SEV_SEG_MAX_MODULES] =	{
													{
														{
															{ SevSeg_u8MOD_1_A_GROUP   , SevSeg_u8MOD_1_A_PIN   }, 	/*	Module 1	PIN A	*/
															{ SevSeg_u8MOD_1_B_GROUP   , SevSeg_u8MOD_1_B_PIN   },	/*	Module 1	PIN B	*/
															{ SevSeg_u8MOD_1_C_GROUP   , SevSeg_u8MOD_1_C_PIN   },	/*	Module 1	PIN C	*/
															{ SevSeg_u8MOD_1_D_GROUP   , SevSeg_u8MOD_1_D_PIN   }	/*	Module 1	PIN D	*/
														},
															{ SevSeg_u8MOD_1_DIP_GROUP , SevSeg_u8MOD_1_DIP_PIN },	/*	Module 1	PIN DIP	*/
															{ SevSeg_u8MOD_1_EN_GROUP  , SevSeg_u8MOD_1_EN_PIN 	},	/*	Module 1	PIN EN	*/
													},
													{
														{
															{ SevSeg_u8MOD_2_A_GROUP   ,SevSeg_u8MOD_2_A_PIN    },	/*	Module 2	PIN A	*/
															{ SevSeg_u8MOD_2_B_GROUP   ,SevSeg_u8MOD_2_B_PIN    },	/*	Module 2	PIN B	*/
															{ SevSeg_u8MOD_2_C_GROUP   ,SevSeg_u8MOD_2_C_PIN    },	/*	Module 2	PIN C	*/
															{ SevSeg_u8MOD_2_D_GROUP   ,SevSeg_u8MOD_2_D_PIN    }	/*	Module 2	PIN D	*/
														},
															{ SevSeg_u8MOD_2_DIP_GROUP ,SevSeg_u8MOD_2_DIP_PIN  },	/*	Module 2	PIN DIP	*/
															{ SevSeg_u8MOD_2_EN_GROUP  ,SevSeg_u8MOD_2_EN_PIN   }	/*	Module 2	PIN EN	*/
													}
												};

