/*
 * TMU_prog.c
 *
 *  Created on: Sep 24, 2022
 *      Author: Bassem El-Behaidy
 */
#include "..\SHARED\ATMEGA32_Registers.h"
#include "..\SHARED\stdTypes.h"
#include "..\SHARED\errorState.h"
#include "..\SHARED\BIT_MATH.h"

#include "..\MCAL\TIMER\TIMER_int.h"

#include "TMU_config.h"
#include "TMU_priv.h"

#if TIMER_CHANNEL == TIMER0
void TMU_vid_OCIE0_ISR (void) ;
void TMU_vid_TOIE0_ISR(void) ;
#elif TIMER_CHANNEL == TIMER2
void TMU_vid_OCIE2_ISR (void) ;
void TMU_vid_TOIE2_ISR(void) ;
#endif

#if MAX_TASKS >0 && MAX_TASKS<=10
#else
#error num of tasks is invalid
#endif

static u8 TMU_u8ISRNum;
static u8 TMU_u8ISRCount;
static u8 TMU_u8Preload;

static TCB_t All_Tasks[MAX_TASKS];
static u32 TMU_u32OsTicks;

void TMU_vidInit(void)
{
	Timer_enuInit();

#if TIMER_CHANNEL == TIMER0
	Timer_enuSetClkPrescaler( TIMER0 , PRES_1024 );
	Timer_enuSetOCn_Mode( TIMER0 , COMP_NORMAL );
	#if (OS_TICK > 0 ) && ( OS_TICK <= 16 )
		if( ES_OK == Timer_enuSetOCRnValue( TIMER0 , (u8)((OS_TICK * CPU_FREQ_KHZ)/1024ul)) )
		{
			Timer_enuSetTimer_Mode( TIMER0 , WGM_CTC_MODE );
			TMU_u8ISRNum = 1;
			TMU_u8ISRCount = 1;
			Timer_enuInterruptEnable( OCIE0 );
			Timer_enuCallBack( OCIE0 , TMU_vid_OCIE0_ISR );
		}
	#elif ( OS_TICK <=150 ) && ( OS_TICK > 0 )
		u8 GCF = 16;
		for (;GCF>0;GCF--)
			if (OS_TICK%GCF == 0)
				break;
		if (GCF > 4)
		{
			Timer_enuSetTimer_Mode( TIMER0 , WGM_CTC_MODE );
			Timer_enuSetOCRnValue( TIMER0 , (u8)((GCF * CPU_FREQ_KHZ )/ 1024ul) ) ;
			TMU_u8ISRNum = (OS_TICK/GCF);
			TMU_u8ISRCount = TMU_u8ISRNum;
			Timer_enuInterruptEnable( OCIE0 );
			Timer_enuCallBack( OCIE0 , TMU_vid_OCIE0_ISR );
		}
		else
		{
			Timer_enuSetTimer_Mode( TIMER0 , WGM_NORMAL_MODE );
			u32 Counts = (OS_TICK * CPU_FREQ_KHZ)/1024ul;
			TMU_u8ISRNum = (Counts + TIMER0_MAX)/( TIMER0_MAX + 1 );
			TMU_u8ISRCount = TMU_u8ISRNum;
			TMU_u8Preload = ( TIMER0_MAX + 1 ) - ( Counts % ( TIMER0_MAX + 1 ) );
			Timer_enuPreLoad( TIMER0 , TMU_u8Preload );
			Timer_enuInterruptEnable( TOIE0 );
			Timer_enuCallBack( TOIE0 , TMU_vid_OCIE0_ISR );
		}
	#else
	#error os tick value is invalid
	#endif
#elif TIMER_CHANNEL == TIMER2
	Timer_enuSetClkPrescaler( TIMER2 , PRES_1024 );
	Timer_enuSetOCn_Mode( TIMER2 , COMP_NORMAL );
	#if OS_TICK >0 && OS_TICK <= 16
		Timer_enuSetTimer_Mode( TIMER2 , WGM_CTC_MODE );
		Timer_enuSetOCRnValue( TIMER2 , (u8)((OS_TICK * CPU_FREQ_KHZ)/1024ul)) ) ;
		TMU_u8ISRNum = 1;
		TMU_u8ISRCount = 1;
		Timer_enuInterruptEnable( OCIE2 );
		Timer_enuCallBack( OCIE2 , TMU_vid_OCIE2_ISR );
	#elif OS_TICK <=150 && OS_TICK > 0
		u8 GCF = 16;
		for (;GCF>0;GCF--)
			if (OS_TICK%GCF == 0)
				break;
		if (GCF > 4)
		{
			Timer_enuSetTimer_Mode( TIMER2 , WGM_CTC_MODE );
			Timer_enuSetOCRnValue( TIMER2 , (u8)((GCF * CPU_FREQ_KHZ )/ 1024ul) ) ;
			TMU_u8ISRNum = (OS_TICK/GCF);
			TMU_u8ISRCount = TMU_u8ISRNum;
			Timer_enuInterruptEnable( OCIE2 );
			Timer_enuCallBack( OCIE2 , TMU_vid_OCIE2_ISR );
		}
		else
		{
			Timer_enuSetTimer_Mode( TIMER2 , WGM_NORMAL_MODE );
			u32 Counts = (OS_TICK * CPU_FREQ_KHZ)/1024ul;
			TMU_u8ISRNum = (Counts + TIMER2_MAX )/ ( TIMER2_MAX + 1 );
			TMU_u8ISRCount = TMU_u8ISRNum;
			TMU_u8Preload = ( TIMER2_MAX + 1 ) - ( Counts % ( TIMER2_MAX + 1 ) );
			Timer_enuPreLoad( TIMER2 , TMU_u8Preload );
			Timer_enuInterruptEnable( TOIE2 );
			Timer_enuCallBack( TOIE2 , TMU_vid_TOIE2_ISR );
		}
	#else
	#error os tick value is invalid
	#endif
#else
#error Timer channel configuration is invalid
#endif
}

void TMU_vidCreateTask( void( *Copy_pFunAppFun )( void* ) , void *Copy_pvidParameter, u8 Copy_u8Priority, u8 Copy_u8State ,u16 Copy_u16Periodicity  , u8 Copy_u8Offset)
{
	if (Copy_pFunAppFun != NULL && Copy_u8Priority < MAX_TASKS)
	{
		All_Tasks[ Copy_u8Priority ].pFun = Copy_pFunAppFun;
		All_Tasks[ Copy_u8Priority ].parameter = Copy_pvidParameter;
		All_Tasks[ Copy_u8Priority ].Periodicity = Copy_u16Periodicity;
		All_Tasks[ Copy_u8Priority ].state = Copy_u8State;
		All_Tasks[ Copy_u8Priority ].offset = Copy_u8Offset;
	}
}

void TMU_vidStartScheduler(void)
{
	u32 Local_u32PrevTick = 0;
	_SEI_;
	while(1)
	{
		if (TMU_u32OsTicks > Local_u32PrevTick )
		{
			Local_u32PrevTick = TMU_u32OsTicks;
			for(s8 i = MAX_TASKS-1 ; i>=0 ; i--)
			{
				if (All_Tasks[i].pFun  != NULL	&&
					All_Tasks[i].state == READY &&
					TMU_u32OsTicks % All_Tasks[i].Periodicity == All_Tasks[i].offset )
				{
					All_Tasks[i].pFun (All_Tasks[i].parameter);
				}
			}
		}
	}
}


void TMU_vidDeleteTask(u8 Copy_u8Priority)
{
	if (Copy_u8Priority <MAX_TASKS)
	{
		All_Tasks[Copy_u8Priority].pFun = NULL;
		All_Tasks[ Copy_u8Priority].state = KILLED;
	}
}

void TMU_vidPauseTask(u8 Copy_u8Priority)
{
	if (Copy_u8Priority <MAX_TASKS)
	{
		All_Tasks[Copy_u8Priority].state = PAUSED;
	}
}

void TMU_vidResumeTask(u8 Copy_u8Priority)
{
	if (Copy_u8Priority <MAX_TASKS)
	{
		All_Tasks[Copy_u8Priority].state = READY;
	}
}

#if TIMER_CHANNEL == TIMER0

void TMU_vid_OCIE0_ISR (void)
{
	TMU_u8ISRCount--;
	if ( ! TMU_u8ISRCount)
	{
		TMU_u32OsTicks++;

		TMU_u8ISRCount = TMU_u8ISRNum;
	}
}

void TMU_vid_TOIE0_ISR(void)
{
	TMU_u8ISRCount--;
	if ( ! TMU_u8ISRCount)
	{
		TCNT0 = TMU_u8Preload;

		TMU_u32OsTicks++;

		TMU_u8ISRCount = TMU_u8ISRNum;
	}
}


#elif TIMER_CHANNEL == TIMER2
void TMU_vid_OCIE2_ISR (void)
{
	TMU_u8ISRCount--;
	if ( ! TMU_u8ISRCount)
	{
		TMU_u32OsTicks++;
		TMU_u8ISRCount = TMU_u8ISRNum;
	}
}
void TMU_vid_TOIE2_ISR(void)
{
	TMU_u8ISRCount--;
	if ( ! TMU_u8ISRCount)
	{
		TCNT2 = TMU_u8Preload;

		TMU_u32OsTicks++;

		TMU_u8ISRCount = TMU_u8ISRNum;
	}
}

#endif
