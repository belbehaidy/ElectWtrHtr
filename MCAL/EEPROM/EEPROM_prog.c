/*
 * EEPROM_prog.c
 *
 *  Created on: Sep 17, 2022
 *      Author: basse
 */

#include "..\..\SHARED\ATMEGA32_Registers.h"
#include "..\..\SHARED\BIT_MATH.h"
#include "..\..\SHARED\stdTypes.h"
#include "..\..\SHARED\errorState.h"

#include "EEPROM_priv.h"

ES_t EEPROM_enuWriteByte( u16 Copy_u16Address , u8 Copy_u8Data )
{
	WAIT_TILL_BIT_IS_CLR( EECR , EEWE ) ;
	WAIT_TILL_BIT_IS_CLR( SPMCR , SPMEN ) ;

	u8 Local_u8CopySREG = SREG ;						// Keeping a copy of Status register SREG
	_CLI_;												// Disable Interrupts

	EEARH = ( Copy_u16Address >> _BYTE_SHIFT_ );		// Writing the higher byte of address in EEARH
	EEARL = (u8)Copy_u16Address;						// Writing the Lower byte of address in EEARL
	EEDR = Copy_u8Data ;								// Writing the data byte in EEDR register
	ASM_SET_BIT( _SFR_EECR_ , EEMWE );					// Setting EEMWE bit in EECR register
	ASM_SET_BIT( _SFR_EECR_ , EEWE  );					// Setting EEWE bit in EECR register

	SREG = Local_u8CopySREG ;
	return ES_OK;
}

ES_t EEPROM_enuReadByte( u16 Copy_u16Address , u8 *Copy_pu8Data )
{
	ES_t Local_enuErrorState = ES_NOK ;

	if( Copy_pu8Data != NULL )
	{
		WAIT_TILL_BIT_IS_CLR( EECR , EEWE ) ;
		EEARH = ( Copy_u16Address >> _BYTE_SHIFT_ );	// Writing the Higher byte of address in EEARH
		EEARL = (u8)Copy_u16Address;					// Writing the Lower byte of address in EEARL
		ASM_SET_BIT( _SFR_EECR_ , EERE );				// Setting EERE bit in EECR register
		* Copy_pu8Data = EEDR ;							// Reading the data byte from EEDR register
		Local_enuErrorState = ES_OK ;
	}
	else Local_enuErrorState = ES_NULL_POINTER ;

	return Local_enuErrorState ;
}
