/*
 * M=main.c
 *
 *  Created on: Sep 24, 2022
 *      Author: Bassem El-Behaidy
 */

#include "../SHARED/stdTypes.h"
#include "../SHARED/errorState.h"

#include "../TMU/TMU_int.h"
#include "../MCAL/DIO/DIO_int.h"
#include "../MCAL/EEPROM/EEPROM_int.h"

#include "../HAL/COOLENT/COOLENT_int.h"
#include "../HAL/HEATER/HEATER_int.h"
#include "../HAL/LM35/LM35_int.h"
#include "../HAL/LD/LD_int.h"
#include "../HAL/Switch/Switch_int.h"
#include "../HAL/SevSeg/SevSeg_int.h"

#include "WtrHtr_config.h"
#include "WtrHtr_priv.h"

OnOff_t PowerStatus = OFF;
OnOff_t HeaterStatus = OFF;
OnOff_t CoolentStatus = OFF;
u8 LedStatus = LD_OFF ;
Temp_t Global_u8TempStatus = AT_SET ;

u8 Global_u8DisplayMode = NORMAL ;
u8 Global_u8SetupSw;
u8 Global_u8TempSetValue ;					// Set Temperature
u8 Global_u8TempValue = 30;					// Actual Temperrature
s8 Global_u8TempError ;						// Difference between Set point and Actual Temp

int
main(void)
{

	u8 SSeg_u8ActiveModule = TEMP_UNITS ;
	u8 Local_u8StoredSetTemp ;

	LD_enuInit();
	Switch_enuInit();
	SevSeg_enuInit();
	LM35_enuInit();
	Heater_enuInit();
	Coolent_enuInit();
	EEPROM_enuReadByte( (u16)SET_ADDRESS , &Local_u8StoredSetTemp );
	if( Local_u8StoredSetTemp == 0 )
	{
		Global_u8TempSetValue = INIT_SET_TEMP ;
		EEPROM_enuWriteByte( (u16)SET_ADDRESS , Global_u8TempSetValue );
	}
	else Global_u8TempSetValue = Local_u8StoredSetTemp ;

	u16 Local_u16TempValue ;
	while( ES_OK != LM35_enuReadTemp( &Local_u16TempValue ) );
		Global_u8TempValue = (u8)Local_u16TempValue ;

	TMU_vidInit();

	TMU_vidCreateTask(DisplayTemperature	, &SSeg_u8ActiveModule	, 7 , PAUSED , 4  , 0 );
	TMU_vidCreateTask(CheckTemperatureStatus, NULL					, 6 , PAUSED , 10 , 0 );
	TMU_vidCreateTask(AdjustHeaterStatus	, NULL					, 5 , PAUSED , 100, 2 );
	TMU_vidCreateTask(AdjustCoolentStatus	, NULL					, 4 , PAUSED , 100, 2 );
	TMU_vidCreateTask(AdjustRedLampStatus	, NULL					, 3 , PAUSED , 50 , 1 );
	TMU_vidCreateTask(CheckIncrementSwitch	, NULL					, 2 , PAUSED , 7  , 3 );
	TMU_vidCreateTask(CheckDecrementSwitch	, NULL					, 1 , PAUSED , 7  , 1 );
	TMU_vidCreateTask(CheckPowerSwitch		, NULL					, 0 , READY  , 7  , 0 );

	TMU_vidStartScheduler();
}

void DisplayTemperature(void *Copy_pu8ActiveModule)
{
	u8 *Local_u8ActiveModule = (u8*)Copy_pu8ActiveModule ;
	u8 Local_u8DisplayValue;
	static u8 BlinkDelay = BLINK_COUNTS ;
	static bool BlinkStatus = FALSE ;

	if( PowerStatus == ON)
	{
		if( Global_u8DisplayMode == NORMAL )			// NORMAL Operation Mode
		{
			Local_u8DisplayValue = Global_u8TempValue ;
			if( BlinkStatus == TRUE ) BlinkStatus = FALSE ;
		}
		else											// SETUP Mode
		{
			Local_u8DisplayValue = Global_u8TempSetValue ;
			BlinkDelay--;
			if(!BlinkDelay)
			{
				BlinkDelay = BLINK_COUNTS;
				BlinkStatus = ( ( BlinkStatus == TRUE )? FALSE : TRUE ) ;
			}
		}

		if( *Local_u8ActiveModule == TEMP_UNITS )
		{
			SevSeg_enuModuleControl( TEMP_TENS , MODULE_DISABLE);
			SevSeg_enuSetDigitValue( TEMP_UNITS , ( Local_u8DisplayValue % 10 ) );
			if( BlinkStatus == FALSE )	SevSeg_enuModuleControl( TEMP_UNITS , MODULE_ENABLE);
			*Local_u8ActiveModule = TEMP_TENS ;
		}
		else
		{
			SevSeg_enuModuleControl( TEMP_UNITS , MODULE_DISABLE);
			SevSeg_enuSetDigitValue( TEMP_TENS , ( Local_u8DisplayValue / 10 ) );
			if( BlinkStatus == FALSE )SevSeg_enuModuleControl( TEMP_TENS , MODULE_ENABLE);
			*Local_u8ActiveModule = TEMP_UNITS ;
		}
	}
	else
	{
		SevSeg_enuModuleControl( TEMP_UNITS , MODULE_DISABLE);
		SevSeg_enuModuleControl( TEMP_TENS , MODULE_DISABLE);
		*Local_u8ActiveModule = TEMP_UNITS;
		TMU_vidPauseTask( 7 );
	}
}

void CheckTemperatureStatus(void *pNULL)
{
	u16 Local_u16TempValue ;
	static u16 Local_u16TempAccValue = 0 ;
	static u8 Local_u8PrevTempValue = 0 ;
	static u8 Local_u8TempReadCounter = TEMP_AVG_READINGS ;

	if( Global_u8DisplayMode == NORMAL && PowerStatus == ON )
	{
		LM35_enuReadTemp( &Local_u16TempValue );
		Local_u16TempAccValue += Local_u16TempValue ;
		ADC_enuStartConversion();
		Local_u8TempReadCounter--;
		if( !Local_u8TempReadCounter )
		{
			Local_u8TempReadCounter = TEMP_AVG_READINGS;
			Global_u8TempValue = Local_u16TempAccValue / 10 ;
			Global_u8TempError = Global_u8TempValue - Local_u8PrevTempValue ;
			Local_u8PrevTempValue = Global_u8TempValue ;
			if( Global_u8TempError >= HTR_TEMP_TOLERANCE )
			{
				if( HeaterStatus == OFF )
				{
					CoolentStatus = ON ;
					LedStatus = LD_ON ;
					TMU_vidResumeTask( 4 );
				}
				else
				{
					HeaterStatus = OFF ;
				}
			}
			else if( Global_u8TempError < -COOLENT_TEMP_TOLERANCE )
			{
				if( CoolentStatus == OFF )
				{
					HeaterStatus = ON ;
					TMU_vidResumeTask( 5 );
					LedStatus = LD_BLINK ;
				}
				else
				{
					CoolentStatus = OFF ;
				}
			}
		}
	}
	else
	{
		LedStatus = LD_OFF ;
		HeaterStatus = OFF ;
		CoolentStatus = OFF ;
		TMU_vidPauseTask( 6 );
	}
}

void CheckIncrementSwitch(void *pNULL)
{
	u8 Local_u8SwitchValue;


		if( ES_OK == Switch_enuGetPressed( INC_SW , &Local_u8SwitchValue ) )
		{
			static u8 press = 0, hold = 0 , BounceDelay = BOUNCE_COUNTS ;
			static u16 SetupDelay = SETUP_COUNTS ;
			if ( (Local_u8SwitchValue == DIO_u8HIGH ) && press == 0 && hold == 0 )  // First press
			{
				if( Global_u8DisplayMode == NORMAL )
				{
					Global_u8DisplayMode = SETUP ;
					Global_u8SetupSw = INC_SW ;
				}
				else
				{
					if( Global_u8TempSetValue < TEMP_MAX_LIMIT )
					{
						Global_u8TempSetValue++;
						press = 1 ;
					}
				}
			}
			else if ( (Local_u8SwitchValue == DIO_u8HIGH ) && press == 1 && hold == 0 ) // Continued first press
			{
				hold = 1;
			}
			else if ( (Local_u8SwitchValue == DIO_u8LOW ) && press == 1 && hold == 1 ) // Removed first press
			{
				BounceDelay--;
				if( !BounceDelay )
				{
					press = 0 ;
					hold = 0 ;
					BounceDelay = BOUNCE_COUNTS ;
				}
			}
			else if (	( Global_u8DisplayMode == SETUP ) &&
						(Local_u8SwitchValue == DIO_u8LOW ) &&
						press == 0 		&&		hold == 0 ) 							// Unpress delay in Setup mode
			{
				SetupDelay--;
				if( !SetupDelay && Global_u8SetupSw == INC_SW )
				{
					EEPROM_enuWriteByte( (u16)SET_ADDRESS , Global_u8TempSetValue );
					Global_u8DisplayMode = NORMAL ;
					SetupDelay = SETUP_COUNTS ;
				}
			}

		}
}
void CheckDecrementSwitch(void *pNULL )
{
	u8 Local_u8SwitchValue;

	if( ES_OK == Switch_enuGetPressed( DEC_SW , &Local_u8SwitchValue ) )
	{
		static u8 press = 0, hold = 0 , BounceDelay = BOUNCE_COUNTS ;
		static u16 SetupDelay = SETUP_COUNTS ;
		if ( (Local_u8SwitchValue == DIO_u8HIGH ) && press == 0 && hold == 0 )  //First press
		{
			if( Global_u8DisplayMode == NORMAL )
			{
				Global_u8DisplayMode = SETUP ;
				Global_u8SetupSw = DEC_SW ;
			}
			else
			{
				if( Global_u8TempSetValue > TEMP_MIN_LIMIT )
				{
					Global_u8TempSetValue--;
					press = 1 ;
				}
			}
		}
		else if ( (Local_u8SwitchValue == DIO_u8HIGH ) && press == 1 && hold == 0 ) //continued first press
		{
			hold = 1;
		}
		else if ( (Local_u8SwitchValue== DIO_u8LOW ) && press == 1 && hold == 1 ) // removed first press
		{
			BounceDelay--;
			if( !BounceDelay )
			{
				press = 0 ;
				hold = 0 ;
				BounceDelay = BOUNCE_COUNTS ;
			}
		}
		else if (	( Global_u8DisplayMode == SETUP ) &&
					(Local_u8SwitchValue == DIO_u8LOW ) &&
					press == 0 		&&		hold == 0 ) // Unpress delay in Setup mode
		{
			SetupDelay--;
			if( !SetupDelay && Global_u8SetupSw == DEC_SW )
			{
				EEPROM_enuWriteByte( (u16)SET_ADDRESS , Global_u8TempSetValue );
				Global_u8DisplayMode = NORMAL ;
				SetupDelay = SETUP_COUNTS ;
			}
		}

	}
}

void CheckPowerSwitch(void *pNULL )
{
	u8 Local_u8SwitchValue;

	if( ES_OK == Switch_enuGetPressed( DEC_SW , &Local_u8SwitchValue ) )
	{
		static u8 press = 0, hold = 0 , BounceDelay = BOUNCE_COUNTS ;
		if ( (Local_u8SwitchValue == DIO_u8HIGH ) && press == 0 && hold == 0 )  //First press
		{
			if( PowerStatus == OFF )
			{
				PowerStatus = ON ;
				TMU_vidResumeTask( 7 );
				TMU_vidResumeTask( 6 );
				TMU_vidResumeTask( 5 );
				TMU_vidResumeTask( 4 );
				TMU_vidResumeTask( 3 );
				TMU_vidResumeTask( 2 );
				TMU_vidResumeTask( 1 );

			}
			else if( PowerStatus == ON )
			{
				PowerStatus = OFF ;
				TMU_vidPauseTask( 2 );
				TMU_vidPauseTask( 1 );
			}
			press = 1 ;
		}
		else if ( (Local_u8SwitchValue == DIO_u8HIGH ) && press == 1 && hold == 0 ) //continued first press
		{
			hold = 1;
		}
		else if ( (Local_u8SwitchValue== DIO_u8LOW ) && press == 1 && hold == 1 ) // removed first press
		{
			BounceDelay--;
			if( !BounceDelay )
			{
				press = 0 ;
				hold = 0 ;
				BounceDelay = BOUNCE_COUNTS ;
			}
		}
	}
}

void AdjustRedLampStatus(void *Copy_pu8RedLampStatus)
{
	static u8 Local_u8PrevStatus = LD_OFF ;
	u8 *Local_u8RedLampStatus = (u8*)Copy_pu8RedLampStatus;

	if( *Local_u8RedLampStatus == LD_BLINK )
	{
		switch( Local_u8PrevStatus )
		{
			case LD_ON	:  Local_u8PrevStatus = LD_OFF ;
			break;
			case LD_OFF	:  Local_u8PrevStatus = LD_ON ;
			break;
		}
		LD_enuSetState( RED_LD , Local_u8PrevStatus );
	}
	else if( *Local_u8RedLampStatus != Local_u8PrevStatus)
	{

		Local_u8PrevStatus = *Local_u8RedLampStatus ;
		LD_enuSetState( RED_LD , Local_u8PrevStatus );
		if( PowerStatus == OFF )
		{
			TMU_vidPauseTask( 3 );
		}
	}
}

void AdjustHeaterStatus( void *pNULL)
{
	if( HeaterStatus == OFF )
	{
		Heater_enuSetState( HTR_TEMP_TOLERANCE + 1 );
		TMU_vidPauseTask( 5 );
	}
	else
		Heater_enuSetState( Global_u8TempError );
}

void AdjustCoolentStatus( void *pNULL)
{
	if( CoolentStatus == OFF )
	{
		Coolent_enuSetState( -COOLENT_TEMP_TOLERANCE - 1 );
		TMU_vidPauseTask( 4 );
	}
	else
		Coolent_enuSetState( Global_u8TempError );
}
