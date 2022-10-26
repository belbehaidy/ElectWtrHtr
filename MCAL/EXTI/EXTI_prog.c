/*
 * EXTI_prog.c
 *
 *  Created on: Aug 19, 2022
 *      Author: Bassem EL-Behaidy
 */
#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"
#include "..\..\SHARED\BIT_MATH.h"
#include "..\..\SHARED\ATMEGA32_Registers.h"

#include "EXTI_priv.h"
#include "EXTI_config.h"

extern u8 EXTI_u8EXTI_Num;

void (*EXTI_ApFunction_AppFun [3] )(void) = { NULL , NULL , NULL } ;

extern 	EXTI_t EXTI_AstrEXTI_Config[] ;


ES_t EXTI_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK;

	for(u8 Local_u8Iter = 0 ; Local_u8Iter < EXTI_u8EXTI_Num ; Local_u8Iter++)
	{
		if( EXTI_AstrEXTI_Config[Local_u8Iter].IntNum == INT0 )						/********************************************************************/
		{																			/*	The Two Bits [ISC01|ISC00] combinations select INT0 Sense Choice*/
																					/********************************************************************/
			MCUCR &= ~(	_TWO_BITS_MASK_	<<	ISC00_BIT	);							/*		Masking the two bits to change according to Choice			*/
			switch(EXTI_AstrEXTI_Config[Local_u8Iter].SenseLvl)						/*																	*/
			{																		/*																	*/
				case	LOW_LEVEL	:											  	/*		Keeping them as '00' for Low Level Sense Choice				*/
										break;										/*																	*/
				case	ANY_LGC_LVL	:	SET_BIT( MCUCR , ISC00_BIT );				/*		Setting them to '01' for Any Logical Level Sense Choice		*/
										break;										/*																	*/
				case	FALLING_EDGE:	SET_BIT( MCUCR , ISC01_BIT );				/*		Setting them to '10' for Falling Edge Sense Choice			*/
										break;										/*																	*/
				case	RISING_EDGE	:	MCUCR |= ( _TWO_BITS_MASK_	<<	ISC00_BIT );/*		Setting them to '11' for Rising Edge Sense Choice			*/
										break;										/********************************************************************/
				default				: Local_enuErrorState = ES_OUT_RANGE;
			}
			if( Local_enuErrorState == ES_OUT_RANGE)	break;						/************************************/
			switch(EXTI_AstrEXTI_Config[Local_u8Iter].IntMode)						/*		PIE0 Sets INT0	mode		*/
			{																		/************************************/
				case	INTERRUPT	:	SET_BIT( GICR ,	PIE0_BIT );					/*			ENABLE INTERRUPT		*/
										break;										/*									*/
				case	POLLING		:	CLR_BIT( GICR ,	PIE0_BIT );					/*			DISABLE INTERRUPT		*/
										break;										/************************************/
				default				:	Local_enuErrorState = ES_OUT_RANGE;
			}
			if( Local_enuErrorState == ES_OUT_RANGE)	break;
		}

		else if( EXTI_AstrEXTI_Config[Local_u8Iter].IntNum == INT1 )				/********************************************************************/
		{																			/*	The Two Bits [ISC11|ISC10] combinations select INT1 Sense Choice*/
																					/********************************************************************/
			MCUCR &= ~(	_TWO_BITS_MASK_	<<	ISC10_BIT	);							/*		Masking the two bits to change according to Choice			*/
			switch(EXTI_AstrEXTI_Config[Local_u8Iter].SenseLvl)						/*																	*/
			{																		/*																	*/
				case	LOW_LEVEL	:											  	/*		Keeping them as '00' for Low Level Sense Choice				*/
										break;										/*																	*/
				case	ANY_LGC_LVL	:	SET_BIT( MCUCR , ISC10_BIT );				/*		Setting them to '01' for Any Logical Level Sense Choice		*/
										break;										/*																	*/
				case	FALLING_EDGE:	SET_BIT( MCUCR , ISC11_BIT );				/*		Setting them to '10' for Falling Edge Sense Choice			*/
										break;										/*																	*/
				case	RISING_EDGE	:	MCUCR |= ( _TWO_BITS_MASK_	<<	ISC10_BIT );/*		Setting them to '11' for Rising Edge Sense Choice			*/
										break;										/********************************************************************/
				default				: Local_enuErrorState = ES_OUT_RANGE;
			}
			if( Local_enuErrorState == ES_OUT_RANGE)	break;						/************************************/
			switch(EXTI_AstrEXTI_Config[Local_u8Iter].IntMode)						/*		PIE0 Sets INT1	mode		*/
			{																		/************************************/
				case	INTERRUPT	:	SET_BIT( GICR ,	PIE1_BIT );					/*			ENABLE INTERRUPT		*/
										break;										/*									*/
				case	POLLING		:	CLR_BIT( GICR ,	PIE1_BIT );					/*			DISABLE INTERRUPT		*/
										break;										/************************************/
				default				:	Local_enuErrorState = ES_OUT_RANGE;
			}
		}
		else if( EXTI_AstrEXTI_Config[Local_u8Iter].IntNum == INT2 )				/********************************************************************/
		{																			/*	 		The ISC2 Bit Value select INT2 Sense Choice				*/
			switch(EXTI_AstrEXTI_Config[Local_u8Iter].SenseLvl)						/********************************************************************/
			{																		/*																	*/
				case	FALLING_EDGE:	CLR_BIT( MCUCSR , ISC2_BIT );				/*			Setting ISC2 to '0' for Falling Edge Sense Choice 		*/
										break;										/*																	*/
				case	RISING_EDGE	:	SET_BIT( MCUCSR , ISC2_BIT );				/*			Setting ISC2 to '1' for Rising Edge Sense Choice 		*/
										break;										/********************************************************************/
				default				: Local_enuErrorState = ES_OUT_RANGE;
			}
			if( Local_enuErrorState == ES_OUT_RANGE)	break;						/************************************/
			switch(EXTI_AstrEXTI_Config[Local_u8Iter].IntMode)						/*		PIE0 Sets INT2	mode		*/
			{																		/************************************/
				case	INTERRUPT	:	SET_BIT( GICR , PIE2_BIT );					/*			ENABLE INTERRUPT		*/
										break;										/*									*/
				case	POLLING		:	CLR_BIT( GICR , PIE2_BIT);					/*			DISABLE INTERRUPT		*/
										break;										/************************************/
				default				:	Local_enuErrorState = ES_OUT_RANGE;
			}
			if( Local_enuErrorState == ES_OUT_RANGE)	break;
		}
		else Local_enuErrorState = ES_OUT_RANGE;
	}
	if( Local_enuErrorState != ES_OUT_RANGE) Local_enuErrorState = ES_OK;

	return Local_enuErrorState ;
}

ES_t EXTI_enuEnableInterrupt( u8 Copy_u8IntNum )
{
	ES_t Local_enuErrorState = ES_NOK;

	if(Copy_u8IntNum == INT0 )
	{																		/********************************/
		SET_BIT( GICR , PIE0_BIT );											/*			ENABLE INT0			*/
	}																		/********************************/
	else if(Copy_u8IntNum == INT1 )
	{																		/********************************/
		SET_BIT( GICR , PIE1_BIT );											/*			ENABLE INT1			*/
	}																		/********************************/
	else if(Copy_u8IntNum == INT2 )
	{																		/********************************/
		SET_BIT( GICR , PIE2_BIT );											/*			ENABLE INT2			*/
	}																		/********************************/
	else Local_enuErrorState = ES_OUT_RANGE;

	if( Local_enuErrorState != ES_OUT_RANGE) Local_enuErrorState = ES_OK;


	return Local_enuErrorState ;
}

ES_t EXTI_enuDisableInterrupt( u8 Copy_u8IntNum )
{
	ES_t Local_enuErrorState = ES_NOK;

	if(Copy_u8IntNum == INT0 )
	{																		/********************************/
		CLR_BIT( GICR , PIE0_BIT );											/*			DISABLE INT0		*/
	}																		/********************************/
	else if(Copy_u8IntNum == INT1 )
	{																		/********************************/
		CLR_BIT( GICR , PIE1_BIT );											/*			DISABLE INT1		*/
	}																		/********************************/
	else if(Copy_u8IntNum == INT2 )
	{																		/********************************/
		CLR_BIT( GICR , PIE2_BIT );											/*			DISABLE INT2		*/
	}																		/********************************/
	else Local_enuErrorState = ES_OUT_RANGE;

	if( Local_enuErrorState != ES_OUT_RANGE) Local_enuErrorState = ES_OK;

	return Local_enuErrorState ;
}

ES_t EXTI_enuSetSenseLevel( u8 Copy_u8IntNum , u8 Copy_u8SenseLevel )
{
	ES_t Local_enuErrorState = ES_NOK ;


	if( Copy_u8IntNum == INT0 )													/********************************************************************/
	{																			/*	The Two Bits [ISC01|ISC00] combinations select INT0 Sense Choice*/
																				/********************************************************************/
		MCUCR &= ~(	_TWO_BITS_MASK_	<<	ISC00_BIT	);							/*		Masking the two bits to change according to Choice			*/
		switch( Copy_u8SenseLevel )												/*																	*/
		{																		/*																	*/
			case	LOW_LEVEL	:											  	/*		Keeping them as '00' for Low Level Sense Choice				*/
									break;										/*																	*/
			case	ANY_LGC_LVL	:	SET_BIT( MCUCR , ISC00_BIT );				/*		Setting them to '01' for Any Logical Level Sense Choice		*/
									break;										/*																	*/
			case	FALLING_EDGE:	SET_BIT( MCUCR , ISC01_BIT );				/*		Setting them to '10' for Falling Edge Sense Choice			*/
									break;										/*																	*/
			case	RISING_EDGE	:	SET_BIT( MCUCR , ISC00_BIT );				/*		Setting them to '11' for Rising Edge Sense Choice			*/
									break;										/********************************************************************/
			default				:	Local_enuErrorState = ES_OUT_RANGE;
		}
	}

	else if( Copy_u8IntNum == INT1 )											/********************************************************************/
	{																			/*	The Two Bits [ISC11|ISC10] combinations select INT1 Sense Choice*/
																				/********************************************************************/
		MCUCR &= ~(	_TWO_BITS_MASK_	<<	ISC10_BIT	);							/*		Masking the two bits to change according to Choice			*/
		switch( Copy_u8SenseLevel )												/*																	*/
		{																		/*																	*/
			case	LOW_LEVEL	:											  	/*		Keeping them as '00' for Low Level Sense Choice				*/
									break;										/*																	*/
			case	ANY_LGC_LVL	:	SET_BIT( MCUCR , ISC10_BIT );				/*		Setting them to '01' for Any Logical Level Sense Choice		*/
									break;										/*																	*/
			case	FALLING_EDGE:	SET_BIT( MCUCR , ISC11_BIT );				/*		Setting them to '10' for Falling Edge Sense Choice			*/
									break;										/*																	*/
			case	RISING_EDGE	:	MCUCR |= ( _TWO_BITS_MASK_	<<	ISC10_BIT );/*		Setting them to '11' for Rising Edge Sense Choice			*/
									break;										/********************************************************************/
			default				:	Local_enuErrorState = ES_OUT_RANGE;
		}
	}
	else if( Copy_u8IntNum == INT2 )											/********************************************************************/
	{																			/*	 		The ISC2 Bit Value select INT2 Sense Choice				*/
		switch( Copy_u8SenseLevel )												/********************************************************************/
		{																		/*																	*/
			case	FALLING_EDGE:	CLR_BIT( MCUCR , ISC2_BIT );				/*			Setting ISC2 to '0' for Falling Edge Sense Choice 		*/
									break;										/*																	*/
			case	RISING_EDGE	:	SET_BIT( MCUCR , ISC2_BIT );				/*			Setting ISC2 to '1' for Rising Edge Sense Choice 		*/
									break;										/********************************************************************/
			default				:	Local_enuErrorState = ES_OUT_RANGE;
		}
	}
	else Local_enuErrorState = ES_OUT_RANGE;

	if( Local_enuErrorState != ES_OUT_RANGE) Local_enuErrorState = ES_OK;

	return Local_enuErrorState ;
}

//	function Name is	"void __vector_n(void)"	, where n= INT_ID = ( Vector No. - 1 ) from Vector Table (Page 42 in DataSheet)
//	Must set attribute to '__attribute((signal))' to inform compiler it is a RETI not a normal RET ,
//	Otherwise Context will not be restored properly and any Extra Interrupts would be disabled as GIE would stay '0'
void __vector_1(void)__attribute__((signal));	//ISR	INT0
void __vector_2(void)__attribute__((signal));	//ISR	INT1
void __vector_3(void)__attribute__((signal));	//ISR	INT2


void __vector_1(void)
{
	if ( EXTI_ApFunction_AppFun[0] != NULL)
	{
		EXTI_ApFunction_AppFun[0]();
	}
}
void __vector_2(void)
{
	if ( EXTI_ApFunction_AppFun[1] != NULL)
	{
		EXTI_ApFunction_AppFun[1]();
	}
}
void __vector_3(void)
{
	if ( EXTI_ApFunction_AppFun[2] != NULL)
	{
		EXTI_ApFunction_AppFun[2]();
	}
}

ES_t EXTI_enuCallBack( u8 Copy_u8IntNum , void (*Copy_pAppFunction)(void) )
{
	ES_t Local_enuErrorState = ES_NOK;

	if( Copy_pAppFunction != NULL)
	{
		EXTI_ApFunction_AppFun[(Copy_u8IntNum -= INT0)] =  Copy_pAppFunction ;

		Local_enuErrorState = ES_OK;
	}
	else Local_enuErrorState = ES_NULL_POINTER;

	return Local_enuErrorState;
}
