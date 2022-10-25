/*
 * ADC_porg.c
 *
 *  Created on: Aug 23, 2022
 *      Author: Bassem El-Behaidy
 */

#include "../../SHARED/stdTypes.h"
#include "../../SHARED/errorState.h"
#include "../../SHARED/BIT_MATH.h"
#include "../../SHARED/ATMEGA32_Registers.h"

#include "ADC_config.h"
#include "ADC_priv.h"


static void(*ADC_pFunISRFun)(void) = NULL ;


ES_t ADC_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK;

	//////////////////////////////////
	// 	Setting Prescalar Factor	//
	//////////////////////////////////
	ADCSRA &= ~(ADC_PRE_SCALAR_BITS_MASK);
#if ( ADC_PRES >= ADC_PRES_2 && ADC_PRES <= ADC_PRES_128 )
	ADCSRA |= ( (ADC_PRES - ADC_PRES_0 ) << ADC_PRE_SCALAR_BITS );
#else
	Local_enuErrorState = ES_OUT_RANGE;
	#error "ADC_enuInit() : Preset-scalar Factor Set Value is Out of Range"
#endif

	//////////////////////////////////
	// Setting Reference Voltage	//
	//////////////////////////////////
	ADMUX &= ~(ADC_REF_SEL_BITS_MASK);
#if ( ADC_VREF >= AREF_REF && ADC_VREF <= INTERNAL_REF )
	ADMUX |= ( (ADC_VREF - AREF_REF) << ADC_REF_SEL_BITS);
#else
	Local_enuErrorState = ES_OUT_RANGE;
	#error 	"ADC_enuInit() : ADC Reference Voltage Set Value is Out of Range"
#endif

	//////////////////////////////////////
	// Setting Output Adjust Direction	//
	//////////////////////////////////////
#if ( ADC_ADJUST == RIGHT_ADJUST )
	CLR_BIT( ADMUX , ADLAR_BIT );
#elif ( ADC_ADJUST == LEFT_ADJUST )
	SET_BIT( ADMUX , ADLAR_BIT );
#else
	Local_enuErrorState = ES_OUT_RANGE;
	#error "ADC_enuInit() : ADC Output Adjust Direction Set Value is Out of Range"
#endif

	//////////////////////////////////////
	//	 Selecting Initial Channel		//
	//////////////////////////////////////
	ADMUX &= ~(ADC_CH_SEL_BITS_MASK);
#if ( ADC_INIT_CHANNEL >= CH_00 && ADC_INIT_CHANNEL <= CH_31 )
	ADMUX |= ( (ADC_INIT_CHANNEL - CH_00) << ADC_CH_SEL_BITS );
#else
	Local_enuErrorState = ES_OUT_RANGE;
	#error "ADC_enuInit() : ADC Initial Channel Set Value is Out of Range"
#endif

	//////////////////////////////////////
	//	 Setting ADC Interrupt Mode		//
	//////////////////////////////////////
#if ( ADC_INTERRUPT_MODE == ADC_POLLING )
	CLR_BIT( ADCSRA , ADC_INT_ENABLE_BIT );
#elif ( ADC_INTERRUPT_MODE == ADC_INTERRUPT )
	SET_BIT( ADCSRA , ADC_INT_ENABLE_BIT );
#else
	Local_enuErrorState = ES_OUT_RANGE;
	#error "ADC_enuInit() : ADC Interrupt Mode Set Value is Out of Range"
#endif

	///////////////////////////////////
	// Selecting ADC Trigger  Source //
	///////////////////////////////////
#if ( ADC_TRIGGER_SOURCE >= FREE_RUNNING && ADC_TRIGGER_SOURCE <= TIMER1_CAPT_EVENT )

	SFIOR &= ~( ADC_TRIGGER_SEL_BITS_MASK );

	SFIOR |= ( (ADC_INIT_CHANNEL - FREE_RUNNING) << ADC_TRIGGER_SEL_BITS );

#else
	Local_enuErrorState = ES_OUT_RANGE;
	#error "ADC_enuInit() : ADC Auto Trigger Source Set Value is Out of Range"
#endif

	//////////////////////////////////
	// Selecting ADC Trigger Mode	//
	//////////////////////////////////
#if ( ADC_TRIGGER_MODE == AUTO_TRIGGER || ADC_TRIGGER_MODE == SINGLE_TRIGGER )

	CLR_BIT( ADCSRA , ADC_AUTO_TRIGGER_EN_BIT );

	#if( ADC_TRIGGER_MODE == AUTO_TRIGGER )
		SET_BIT( ADCSRA , ADC_AUTO_TRIGGER_EN_BIT );
	#endif

#else
	Local_enuErrorState = ES_OUT_RANGE;
	#error "ADC_enuInit : ADC Trigger Mode Set Value is Out of Range"
#endif

	//////////////////////////////////
	//	 ENABLE ADC Peripheral		//
	//////////////////////////////////
	SET_BIT( ADCSRA , ADC_ENABLE_BIT );

	if( Local_enuErrorState != ES_OUT_RANGE)
		Local_enuErrorState = ES_OK ;

	return Local_enuErrorState;
}

ES_t ADC_enuSetPreScalar(u8 Copy_u8PreScalarID)
{
	ES_t Local_enuErrorState = ES_NOK;

	ADCSRA &= ~(ADC_PRE_SCALAR_BITS_MASK);

	if ( Copy_u8PreScalarID >= ADC_PRES_2 && Copy_u8PreScalarID <= ADC_PRES_128 )
	{
		ADCSRA |= ( (Copy_u8PreScalarID - ADC_PRES_0 ) << ADC_PRE_SCALAR_BITS );
		Local_enuErrorState = ES_OK;
	}
	else
		Local_enuErrorState = ES_OUT_RANGE;


	return Local_enuErrorState;
}

ES_t ADC_enuSetRefVolt(u8 Copy_u8RefVoltID)
{
	ES_t Local_enuErrorState = ES_NOK;

	ADMUX &= ~(ADC_REF_SEL_BITS_MASK);

	if ( Copy_u8RefVoltID >= AREF_REF && Copy_u8RefVoltID <= INTERNAL_REF )
	{
		ADMUX |= ( (Copy_u8RefVoltID - AREF_REF) << ADC_REF_SEL_BITS);
		Local_enuErrorState = ES_OK;
	}
	else
		Local_enuErrorState = ES_OUT_RANGE;


	return Local_enuErrorState;
}


ES_t ADC_enuSelectChannel(u8 Copy_u8ChannelID)
{
	ES_t Local_enuErrorState = ES_NOK;

	if( Copy_u8ChannelID >= CH_00 && Copy_u8ChannelID <= CH_31 )
	{
		ADMUX &= ~( ADC_CH_SEL_BITS_MASK);
		ADMUX |= ( ( Copy_u8ChannelID - CH_00 ) << ADC_CH_SEL_BITS );
//		ADMUX |= (Copy_u8ChannelID << ADC_CH_SEL_BITS);
		Local_enuErrorState = ES_OK ;
	}
	else Local_enuErrorState = ES_OUT_RANGE;

	return Local_enuErrorState;
}

ES_t ADC_enuStartConversion(void)
{
	ASM_SET_BIT( _SFR_ADCSRA_ , ADC_START_CONVERSION_BIT );

	return ES_OK;
}

ES_t ADC_enuEnableAutoTrigger(u8 Copy_u8TriggerSource)
{
	ES_t Local_enuErrorState = ES_NOK;

	if(Copy_u8TriggerSource >= FREE_RUNNING && Copy_u8TriggerSource <= TIMER1_CAPT_EVENT )
	{
		ASM_CLR_BIT( _SFR_ADCSRA_ , ADC_AUTO_TRIGGER_EN_BIT );

		SFIOR &= ~( ADC_TRIGGER_SEL_BITS_MASK );
		SFIOR |= ( ( Copy_u8TriggerSource - FREE_RUNNING ) << ADC_TRIGGER_SEL_BITS );

		ASM_SET_BIT( _SFR_ADCSRA_ , ADC_AUTO_TRIGGER_EN_BIT );

		Local_enuErrorState = ES_OK;
	}
	else Local_enuErrorState = ES_OUT_RANGE ;

	return Local_enuErrorState;
}

ES_t ADC_enuDisableAutoTrigger(void)
{
	ASM_CLR_BIT( _SFR_ADCSRA_ , ADC_AUTO_TRIGGER_EN_BIT );
//	ADCSRA &= ~(BIT_MASK << ADC_AUTO_TRIGGER_EN_BIT );

	return ES_OK;
}


ES_t ADC_enuRead(u16 *Copy_u16ADC_Value)
{
	ES_t Local_enuErrorState = ES_NOK;

	if( Copy_u16ADC_Value != NULL )
	{
		#if ( ADC_ADJUST == RIGHT_ADJUST)

			*Copy_u16ADC_Value  = ADCL;
			*Copy_u16ADC_Value |= ( (u16)ADCH << 8 );

		#elif ( ADC_ADJUST == LEFT_ADJUST)

			*Copy_u16ADC_Value  = ( ADCL >> 6 );
			*Copy_u16ADC_Value |= ( (u16)ADCH << 2 );

			#warning "ADC_enuRead(u16*): Optimumt Way to read 10-bit Value is to set ADC_ADJUST to RIGHT_ADJUST"

		#endif

		Local_enuErrorState = ES_OK ;
	}
	else Local_enuErrorState = ES_NULL_POINTER ;

	return Local_enuErrorState;
}

ES_t ADC_enuReadHigh(u8 *Copy_u8ADC_Value)

{
	ES_t Local_enuErrorState = ES_NOK;

	if( Copy_u8ADC_Value != NULL )
	{
		#if ( ADC_ADJUST == RIGHT_ADJUST)

			*Copy_u8ADC_Value  = ( ADCL >> 2 );
			*Copy_u8ADC_Value |= ( ADCH << 6 );

			#warning "ADC_enuReadHigh(u8*): Optimum Way to read High is to set ADC_ADJUST to LEFT_ADJUST"

		#elif ( ADC_ADJUST == LEFT_ADJUST)

			*Copy_u8ADC_Value = ADCH ;

		#endif

		Local_enuErrorState = ES_OK ;
	}
	else Local_enuErrorState = ES_NULL_POINTER ;

	return Local_enuErrorState;
}

ES_t ADC_enuPollingRead(u16 *Copy_u16ADC_Value)
{
	ES_t Local_enuErrorState = ES_NOK;

	WAIT_TILL_BIT_IS_SET( ADCSRA , ADC_INT_FLAG_BIT );

	if( Copy_u16ADC_Value != NULL)
	{
#if ( ADC_ADJUST == RIGHT_ADJUST)

		*Copy_u16ADC_Value  = ADCL;
		*Copy_u16ADC_Value |= ( (u16)ADCH << 8 );

#elif ( ADC_ADJUST == LEFT_ADJUST)

		*Copy_u16ADC_Value  = ( ADCL >> 6 );
		*Copy_u16ADC_Value |= ( (u16)ADCH << 2 );

		#warning "ADC_enuRead(u16*): Optimum Way to read 10-bit Value is to set ADC_ADJUST to RIGHT_ADJUST"

#endif
		Local_enuErrorState = ES_OK ;
	}
	else Local_enuErrorState = ES_NULL_POINTER ;

	SET_BIT( ADCSRA , ADC_INT_FLAG_BIT );

	return Local_enuErrorState;
}

ES_t ADC_enuPollingReadHigh(u8 *Copy_u8ADC_Value)

{
	ES_t Local_enuErrorState = ES_NOK;

	WAIT_TILL_BIT_IS_SET( ADCSRA , ADC_INT_FLAG_BIT );

	if( Copy_u8ADC_Value != NULL)
	{
		#if ( ADC_ADJUST == RIGHT_ADJUST)

			*Copy_u8ADC_Value  = ( ADCL >> 2 );
			*Copy_u8ADC_Value |= ( ADCH << 6 );

			#warning "ADC_enuReadHigh(u8*): Optimum Way to read High is to set ADC_ADJUST to LEFT_ADJUST"

		#elif ( ADC_ADJUST == LEFT_ADJUST)

			*Copy_u8ADC_Value = ADCH ;

		#endif

		Local_enuErrorState = ES_OK ;
	}
	else Local_enuErrorState = ES_NULL_POINTER ;

	WAIT_TILL_BIT_IS_SET( ADCSRA , ADC_INT_FLAG_BIT );

	return Local_enuErrorState;
}


ES_t ADC_enuCallBack(void ( *Copy_pFunAppFun )(void))
{
	ES_t Local_enuErrorState = ES_NOK;

	if( Copy_pFunAppFun != NULL)
	{
		ADC_pFunISRFun = Copy_pFunAppFun;
		Local_enuErrorState = ES_OK;
	}
	else Local_enuErrorState = ES_NULL_POINTER;

	return Local_enuErrorState;
}

ES_t ADC_enuEnable(void)
{

	SET_BIT( ADCSRA , ADC_ENABLE_BIT );
//	ADCSRA |= ( BIT_MASK << ADC_ENABLE_BIT );
	return ES_OK;
}

ES_t ADC_enuDisable(void)

{
	CLR_BIT( ADCSRA , ADC_ENABLE_BIT );
//	ADCSRA &= ~( BIT_MASK << ADC_ENABLE_BIT );
	return ES_OK;
}

ES_t ADC_enuEnableInterrupt(void)
{
	SET_BIT( ADCSRA , ADC_ENABLE_BIT );
//	ADCSRA |= ( BIT_MASK << ADC_ENABLE_BIT );
	return ES_OK;
}

ES_t ADC_enuDisableInterrupt(void)
{
	CLR_BIT( ADCSRA , ADC_ENABLE_BIT );
//	ADCSRA &= ~( BIT_MASK << ADC_ENABLE_BIT );
	return ES_OK;
}


void __vector_16(void)__attribute__((signal));
void __vector_16(void)
{
	if( ADC_pFunISRFun != NULL)
		ADC_pFunISRFun();
}

