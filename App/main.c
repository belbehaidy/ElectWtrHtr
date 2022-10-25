/*
 * M=main.c
 *
 *  Created on: Sep 24, 2022
 *      Author: Bassem El-Behaidy
 */

#include "../SHARED/ATMEGA32_Registers.h"
#include "../SHARED/stdTypes.h"
#include "../SHARED/errorState.h"
#include "../SHARED/BIT_MATH.h"

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
Process_t ProcessStatus = HEATING ;
OnOff_t HeaterStatus = OFF;
OnOff_t CoolentStatus = OFF;
u8 LedStatus = LD_OFF ;

u8 Global_u8DisplayMode = NORMAL ;
u8 Global_u8SetupSw;
u16 EEPROM_SetTempAddress = (u16)SET_ADDRESS ;
u8 SetTempSaveID = 0x55 ;
bool SetTempUpdate = TRUE ;

u8 Global_u8TempSetValue ;					// Set Temperature
u8 Global_u8TempValue = 30;					// Actual Temperature
s8 Global_s8TempError ;						// Difference between Set point and Actual Temp

int
main(void)
{

	u8 SSeg_u8ActiveModule = TEMP_UNITS ;

	LD_enuInit();
	Switch_enuInit();
	SevSeg_enuInit();
	LM35_enuInit();
	Heater_enuInit();
	Coolent_enuInit();
	TMU_vidInit();

	EEPROM_Access( &EEPROM_SetTempAddress );

	TMU_vidCreateTask(EEPROM_Access			, &EEPROM_SetTempAddress, 8 , READY  , 3  , 0  );
	TMU_vidCreateTask(DisplayTemperature	, &SSeg_u8ActiveModule	, 7 , PAUSED , 1  , 0  );
	TMU_vidCreateTask(CheckTemperatureStatus, NULL					, 6 , PAUSED , 10 , 0  );
	TMU_vidCreateTask(AdjustHeaterStatus	, NULL					, 5 , PAUSED , 100, 11 );
	TMU_vidCreateTask(AdjustCoolentStatus	, NULL					, 4 , PAUSED , 100, 13 );
	TMU_vidCreateTask(AdjustRedLampStatus	, NULL					, 3 , PAUSED , 50 , 17 );
	TMU_vidCreateTask(CheckIncrementSwitch	, NULL					, 2 , PAUSED , 2  , 0  );
	TMU_vidCreateTask(CheckDecrementSwitch	, NULL					, 1 , PAUSED , 2  , 1  );
	TMU_vidCreateTask(CheckPowerSwitch		, NULL					, 0 , READY  , 2  , 0  );

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
			SevSeg_enuModuleDisable( TEMP_TENS );
			SevSeg_enuSetDigitValue( Local_u8DisplayValue % 10 );
			if( BlinkStatus == FALSE )	SevSeg_enuModuleEnable( TEMP_UNITS );
			*Local_u8ActiveModule = TEMP_TENS ;
		}
		else
		{
			SevSeg_enuModuleDisable( TEMP_UNITS );
			SevSeg_enuSetDigitValue( Local_u8DisplayValue / 10 );
			if( BlinkStatus == FALSE )SevSeg_enuModuleEnable( TEMP_TENS );
			*Local_u8ActiveModule = TEMP_UNITS ;
		}
	}
	else
	{
		SevSeg_enuModuleDisable( TEMP_UNITS );
		SevSeg_enuModuleDisable( TEMP_TENS );
		*Local_u8ActiveModule = TEMP_UNITS;
		TMU_vidPauseTask( 7 );
	}
}

void CheckTemperatureStatus(void *pNULL)
{
	u8 Local_u8TempValue ;
	static u16 Local_u16TempAccValue = 0 ;
	static u8 Local_u8PrevTempValue = 0 ;
	static u8 Local_u8TempReadCounter = TEMP_AVG_READINGS ;

	if( Global_u8DisplayMode == NORMAL && PowerStatus == ON )
	{
		LM35_enuReadTemp( &Local_u8TempValue );
		Local_u16TempAccValue += Local_u8TempValue ;
		ADC_enuStartConversion();
		Local_u8TempReadCounter--;
		if( !Local_u8TempReadCounter )
		{
			Global_u8TempValue = Local_u16TempAccValue / TEMP_AVG_READINGS ;
			Global_s8TempError = Global_u8TempValue - Global_u8TempSetValue ;
			Local_u8PrevTempValue = Global_u8TempValue ;
			Local_u16TempAccValue = 0 ;
			Local_u8TempReadCounter = TEMP_AVG_READINGS;

			if( Global_s8TempError < HTR_TEMP_TOLERANCE && HeaterStatus == OFF && CoolentStatus == OFF )
			{
				ProcessStatus = HEATING ;
				HeaterStatus = ON ;
				CoolentStatus = OFF ;
				LedStatus = LD_BLINK ;
				TMU_vidResumeTask( 5 );
			}
			else if( Global_s8TempError >= HTR_TEMP_TOLERANCE && ProcessStatus == HEATING )
			{
				ProcessStatus = COOLING ;
				HeaterStatus = OFF ;
				CoolentStatus = ON ;
				LedStatus = LD_ON ;
				TMU_vidResumeTask( 4 );
			}
			else if( Global_s8TempError <= -COOLENT_TEMP_TOLERANCE && ProcessStatus == COOLING )
			{
				ProcessStatus = HEATING ;
				HeaterStatus = ON ;
				CoolentStatus = OFF ;
				LedStatus = LD_BLINK ;
				TMU_vidResumeTask( 5 );
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
			static u8 press = 0 , BounceDelay = BOUNCE_COUNTS ;
			static u16 SetupDelay = SETUP_COUNTS ;
			if ( (Local_u8SwitchValue == DIO_u8HIGH ) && press == 0 )  // First press
			{
				if( Global_u8DisplayMode == NORMAL )
				{
					Global_u8DisplayMode = SETUP ;
					Global_u8SetupSw = INC_SW ;
					SetupDelay = SETUP_COUNTS;
				}
				else
				{
					if( Global_u8SetupSw != INC_SW )
						Global_u8SetupSw = INC_SW ;
					if( Global_u8TempSetValue < TEMP_MAX_LIMIT )
					{
						Global_u8TempSetValue += SETUP_STEP;
						press = 1 ;
					}
				}
			}
			else if ( (Local_u8SwitchValue == DIO_u8LOW ) && press == 1 ) // Removed first press
			{
				BounceDelay--;
				if( !BounceDelay )
				{
					press = 0 ;
					BounceDelay = BOUNCE_COUNTS ;
					SetupDelay = SETUP_COUNTS ;
				}
			}
			else if (	( Global_u8DisplayMode == SETUP )	&&  ( Global_u8SetupSw == INC_SW ) &&
						(Local_u8SwitchValue == DIO_u8LOW ) &&	press == 0 ) // Unpress delay in Setup mode
			{
				SetupDelay--;
				if( !SetupDelay )
				{
					SetTempUpdate = TRUE ;
					TMU_vidResumeTask( 6 );
					TMU_vidResumeTask( 8 );
					Global_u8DisplayMode = NORMAL ;
				}
			}

		}
}
void CheckDecrementSwitch(void *pNULL )
{
	u8 Local_u8SwitchValue;

	if( ES_OK == Switch_enuGetPressed( DEC_SW , &Local_u8SwitchValue ) )
	{
		static u8 press = 0 , BounceDelay = BOUNCE_COUNTS ;
		static u16 SetupDelay = SETUP_COUNTS ;
		if ( (Local_u8SwitchValue == DIO_u8HIGH ) && press == 0 )  //First press
		{
			if( Global_u8DisplayMode == NORMAL )
			{
				Global_u8DisplayMode = SETUP ;
				Global_u8SetupSw = DEC_SW ;
			}
			else
			{
				if( Global_u8SetupSw != DEC_SW )
					Global_u8SetupSw = DEC_SW ;
				if( Global_u8TempSetValue > TEMP_MIN_LIMIT )
				{
					Global_u8TempSetValue -= SETUP_STEP;
					press = 1 ;
				}
			}
		}
		else if ( (Local_u8SwitchValue== DIO_u8LOW ) && press == 1 ) // removed first press
		{
			BounceDelay--;
			if( !BounceDelay )
			{
				press = 0 ;
				BounceDelay = BOUNCE_COUNTS ;
				SetupDelay = SETUP_COUNTS;
			}
		}
		else if (	( Global_u8DisplayMode == SETUP )	&&  ( Global_u8SetupSw == DEC_SW ) &&
					(Local_u8SwitchValue == DIO_u8LOW ) &&	press == 0 	 ) // Unpress delay in Setup mode
		{
			SetupDelay--;
			if( !SetupDelay )
			{
				SetTempUpdate = TRUE ;
				TMU_vidResumeTask( 6 );
				TMU_vidResumeTask( 8 );
				Global_u8DisplayMode = NORMAL ;
			}
		}

	}
}

void CheckPowerSwitch(void *pNULL )
{
	u8 Local_u8SwitchValue;

	if( ES_OK == Switch_enuGetPressed( PWR_SW , &Local_u8SwitchValue ) )
	{
		static u8 press = 0, hold = 0 , BounceDelay = BOUNCE_COUNTS ;

		if ( (Local_u8SwitchValue == DIO_u8HIGH ) && press == 0 && hold == 0 )  //First press
		{
			press = 1 ;
			if( PowerStatus == OFF )
			{
				PowerStatus = ON ;
				Global_u8DisplayMode = NORMAL ;
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
	static OnOff_t PrevStatus = OFF ;

	Heater_enuSetState( Global_s8TempError );

	if( HeaterStatus != PrevStatus)
	{
		if( HeaterStatus == OFF )
		{
			Heater_enuDisable();
			TMU_vidPauseTask( 5 );
		}
		else
		{
			Heater_enuEnable();
		}
		PrevStatus = HeaterStatus ;
	}
}

void AdjustCoolentStatus( void *pNULL)
{
	static OnOff_t PrevStatus = OFF ;

	Coolent_enuSetState( Global_s8TempError );

	if( CoolentStatus != PrevStatus)
	{
		if( CoolentStatus == OFF )
		{
			Coolent_enuDisable();
			TMU_vidPauseTask( 4 );
		}
		else
		{
			Coolent_enuEnable();
		}
		PrevStatus = CoolentStatus ;
	}

}

void EEPROM_Access( void *Copy_u16SetTempAddress )
{
	u16 *Local_u16TempAddress = (u16*)Copy_u16SetTempAddress ;
	static u8 Local_u8StoredSetTemp = INIT_SET_TEMP , Local_u8Stage = 0 , Local_u8SetTempSaveID;
	static ES_t Local_enuErrorState = ES_NOK ;

	if( SetTempUpdate == TRUE )
	{
		if( ( Local_u8StoredSetTemp != Global_u8TempSetValue) && ( Local_u8Stage == 0 ) )
		{
			Local_enuErrorState = EEPROM_enuReadByte( *Local_u16TempAddress , &Local_u8SetTempSaveID ) ;
			Local_u8Stage++ ;
		}
		else if( Local_enuErrorState == ES_OK && Local_u8Stage == 1)
		{
			if( Local_u8SetTempSaveID != SetTempSaveID )
			{
				Local_enuErrorState = EEPROM_enuWriteByte( *Local_u16TempAddress , SetTempSaveID ) ;
				Global_u8TempSetValue = INIT_SET_TEMP ;
			}
			Local_u8Stage++ ;
		}
		else if( Local_enuErrorState == ES_OK && Local_u8Stage == 2)
		{
			Local_enuErrorState = EEPROM_enuWriteByte( *( Local_u16TempAddress + 1 ) , Global_u8TempSetValue ) ;
			Local_u8Stage++ ;
		}
		else if( Local_enuErrorState == ES_OK && Local_u8Stage == 3)
		{
			Local_u8StoredSetTemp = Global_u8TempSetValue ;
			SetTempUpdate = FALSE ;
			Local_u8Stage = 0 ;
			TMU_vidPauseTask( 8 );
		}
	}

/*	u16 *Local_u16TempAddress = (u16*)Copy_u16SetTempAddress ;
	static u8 Local_u8StoredSetTemp = 0 ;
	u8 Local_u8SetTempSaveID ;

	if( Local_u8StoredSetTemp != Global_u8TempSetValue)
	{
		if( ES_OK == EEPROM_enuReadByte( *Local_u16TempAddress , &Local_u8SetTempSaveID ) )
		{
			if( Local_u8SetTempSaveID != SetTempSaveID )
			{
				Global_u8TempSetValue = INIT_SET_TEMP ;
				if( ES_OK == EEPROM_enuWriteByte( *( Local_u16TempAddress + 1 ) , Global_u8TempSetValue ) )
				EEPROM_enuWriteByte( *Local_u16TempAddress , SetTempSaveID );
			}
			else
			{
				EEPROM_enuWriteByte( *( Local_u16TempAddress + 1 ) , Global_u8TempSetValue );
			}
		}
		Local_u8StoredSetTemp = Global_u8TempSetValue ;
	}
	TMU_vidPauseTask( 8 );
*/
}
