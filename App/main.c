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

OnOff_t PowerStatus = OFF;						//	is a Flag indicating if POWER is ON or OFF
Process_t ProcessStatus = HEATING ;				//	is a Flag indicating either system in either HEATING or COOLING cycle
OnOff_t HeaterStatus = OFF;						//	is a Flag indicating either Heater is ON or OFF
OnOff_t CoolentStatus = OFF;					//	is a Flag indicating either Coolent is ON or OFF
u8 LedStatus = LD_OFF ;							//	is a Flag indicating LED is ON / OFF / BLINK

u8 Global_u8DisplayMode = NORMAL ;				//	is a Flag indicating either Seven Segment is displaying Actual (NORMAL) or Set (SETUP) temperature
u8 Global_u8SetupSw;							//	is a Flag indicating which switch is active during Setup mode INC_SW /DEC_SW
u16 EEPROM_SetTempAddress = (u16)SET_ADDRESS ;	//	A variable saving SET Temperature address in EEPROM
u8 SetTempSaveID = SAVED_ID ;					//	A variable saving SET TEMP SAVED ID which indicates that EEPROM have a set Temperature saved or Not
bool SetTempUpdate = FALSE ;					//	is a Flag indicating either Set Temperature is updated or not

u8 Global_u8TempSetValue ;						//	is a variable saving Set Temperature Value
u8 Global_u8TempValue = 30;						//	is a variable saving Actual Temperature Value
s8 Global_s8TempError ;							//	is a variable saving Difference between Set Temperature and Actual Temperature

int
main(void)
{

	u8 SSeg_u8ActiveModule = TEMP_UNITS ;

	Heater_enuInit();
	Coolent_enuInit();
	LD_enuInit();
	Switch_enuInit();
	SevSeg_enuInit();
	LM35_enuInit();
	TMU_vidInit();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	Creating Tasks to start Time Management Unit Operation
	//	Tasks are created by calling TMU_vidCreateTask() API which has the following declaration
	//	void TMU_vidCreateTask(
	//		void( *Copy_pFunAppFun )( void* ) ,	->	Pointer to function has a pointer to void argument and returns void
	//		void *Copy_pvidParameter,			->	Pointer to function argument passed as pointer to void
	//		u8 Copy_u8Priority,					->	Value indicating priority of the task, Tasks can not have the same priority,
	//												the higher the number , the higher the priority.
	//												Priority number is the index of the task in the Tasks array
	//		u8 Copy_u8State ,					->	Initial State of the task ( READY / PAUSED ), we have a third state KILLED
	//		u16 Copy_u16Periodicity,			->	The number of Tick counts after which the task executes again.
	//		u8 Copy_u8Offset					->	The number of Tick counts task is delayed after reaching its Periodic Tick before executing.
	//							);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				TMU_vidCreateTask(EEPROM_Access			, &EEPROM_SetTempAddress, 8 , READY	 , 3  , 0  );
				TMU_vidCreateTask(DisplayTemperature	, &SSeg_u8ActiveModule	, 7 , PAUSED , 1  , 0  );
				TMU_vidCreateTask(CheckTemperatureStatus, NULL					, 6 , PAUSED , 10 , 0  );
				TMU_vidCreateTask(AdjustHeaterStatus	, NULL					, 5 , PAUSED , 100, 11 );
				TMU_vidCreateTask(AdjustCoolentStatus	, NULL					, 4 , PAUSED , 100, 13 );
				TMU_vidCreateTask(AdjustRedLampStatus	, NULL					, 3 , PAUSED , 50 , 17 );
				TMU_vidCreateTask(CheckIncrementSwitch	, NULL					, 2 , PAUSED , 2  , 0  );
				TMU_vidCreateTask(CheckDecrementSwitch	, NULL					, 1 , PAUSED , 2  , 1  );
				TMU_vidCreateTask(CheckPowerSwitch		, NULL					, 0 , READY  , 2  , 0  );

	// Starting Scheduler for Time Management Unit
				TMU_vidStartScheduler();
}

void DisplayTemperature(void *Copy_pu8ActiveModule)		//	TASK( 7 )
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

void CheckTemperatureStatus(void *pNULL)		//	TASK( 6 )
{
	u8 Local_u8TempValue ;
	static u16 Local_u16TempAccValue = 0 ;
	static u8 Local_u8PrevTempValue = 0 ;
	static u8 Local_u8TempReadCounter = (u8)TEMP_AVG_READINGS ;

	if( Global_u8DisplayMode == NORMAL && PowerStatus == ON )
	{
		if( ES_OK == LM35_enuReadTemp( &Local_u8TempValue ) )
		{
			Local_u16TempAccValue += Local_u8TempValue ;
			if( ES_OK == ADC_enuStartConversion() )
				Local_u8TempReadCounter--;
		}
		if( !Local_u8TempReadCounter )
		{
			Global_u8TempValue = ( Local_u16TempAccValue / TEMP_AVG_READINGS );
			Global_s8TempError = Global_u8TempValue - Global_u8TempSetValue ;
			Local_u8PrevTempValue = Global_u8TempValue ;
			Local_u16TempAccValue = 0 ;
			Local_u8TempReadCounter = (u8)TEMP_AVG_READINGS;

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

void CheckIncrementSwitch(void *pNULL)		//	TASK( 2 )
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
					press = 1 ;
				}
				else
				{
					if( Global_u8SetupSw != INC_SW )
						Global_u8SetupSw = INC_SW ;
					if( Global_u8TempSetValue <= ( TEMP_MAX_LIMIT - SETUP_STEP ) )
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
void CheckDecrementSwitch(void *pNULL )		//	TASK( 1 )
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
				press = 1 ;
			}
			else
			{
				if( Global_u8SetupSw != DEC_SW )
					Global_u8SetupSw = DEC_SW ;
				if( Global_u8TempSetValue >= ( TEMP_MIN_LIMIT+SETUP_STEP ) )
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

void CheckPowerSwitch(void *pNULL )		//	TASK( 0 )
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

void AdjustRedLampStatus(void *pNULL )		//	TASK( 3 )
{
	static u8 Local_u8PrevStatus = LD_OFF ;

	if( LedStatus == LD_BLINK )
	{
		LD_enuBlink( RED_LD );
	}
	else if( LedStatus != Local_u8PrevStatus)
	{
		Local_u8PrevStatus = LedStatus ;
		LD_enuSetState( RED_LD , Local_u8PrevStatus );
		if( PowerStatus == OFF )
		{
			TMU_vidPauseTask( 3 );
		}
	}
}

void AdjustHeaterStatus( void *pNULL)		//	TASK( 5 )
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

void AdjustCoolentStatus( void *pNULL)		//	TASK( 4 )
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

void EEPROM_Access( void *Copy_u16SetTempAddress )		//	TASK( 8 )
{
	u16 *Local_u16TempAddress = (u16*)Copy_u16SetTempAddress ;
	static u8 Local_u8StoredSetTemp = INIT_SET_TEMP ;
	static u8 Local_u8SetTempSaveID;

	if( Local_u8StoredSetTemp != Global_u8TempSetValue)
	{
		if( ES_OK == EEPROM_enuReadByte( *Local_u16TempAddress , &Local_u8SetTempSaveID ) )
		{
			if( Local_u8SetTempSaveID != SetTempSaveID )
			{
				Global_u8TempSetValue = INIT_SET_TEMP ;
				if( ES_OK == EEPROM_enuWriteByte( (*( Local_u16TempAddress ) + 1 ) , Global_u8TempSetValue ) )
				EEPROM_enuWriteByte( *Local_u16TempAddress , SetTempSaveID );
			}
			else
			{
				if( SetTempUpdate == TRUE )
				{
					if( ES_OK == EEPROM_enuWriteByte( (*( Local_u16TempAddress ) + 1 ) , Global_u8TempSetValue ) )
						SetTempUpdate = FALSE ;
				}
				else
				{
					EEPROM_enuReadByte( (*( Local_u16TempAddress ) + 1 ) , &Global_u8TempSetValue );
				}
			}
		}
		Local_u8StoredSetTemp = Global_u8TempSetValue ;
	}
	TMU_vidPauseTask( 8 );
}
