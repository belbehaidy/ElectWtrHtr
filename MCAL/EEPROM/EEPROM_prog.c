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
	while( ( ( EECR >> EEWE ) & _BIT_MASK_ ) );
	while( ( ( SPMCR >> SPMEN ) & _BIT_MASK_ ) );

	u8 Local_u8CopySREG = SREG ;						// Keeping a copy of Status register SREG
	_CLI_;												// Disable Interrupts

	EEARH = ( Copy_u16Address >> _BYTE_SHIFT_ );		// Writing the higher byte of address in EEARH
	EEARL = (u8)Copy_u16Address;						// Writing the Lower byte of address in EEARL
	EEDR = Copy_u8Data ;								// Writing the data byte in EEDR register
//	ASM_SET_BIT( _SFR_EECR_ , EEMWE );
//	ASM_SET_BIT( _SFR_EECR_ , EEWE  );
	asm( " SBI 0x1C,2 " );								// Setting EEMWE bit in EECR register
	asm( " SBI 0x1C,1 " );								// Setting EEWE bit in EECR register

	SREG = Local_u8CopySREG ;
	return ES_OK;
}

ES_t EEPROM_enuReadByte( u16 Copy_u16Address , u8 *Copy_pu8Data )
{
	ES_t Local_enuErrorState = ES_NOK ;

	if( Copy_pu8Data != NULL )
	{
		while( ( ( EECR >> EEWE ) & 1 ) );
		EEARH = ( Copy_u16Address >> _BYTE_SHIFT_ );	// Writing the Higher byte of address in EEARH
		EEARL = (u8)Copy_u16Address;					// Writing the Lower byte of address in EEARL
//		ASM_SET_BIT( _SFR_EECR_ , EERE );
		asm( " SBI 0x1C,0 " );							// Setting EERE bit in EECR register
		* Copy_pu8Data = EEDR ;							// Reading the data byte from EEDR register
		Local_enuErrorState = ES_OK ;
	}
	else Local_enuErrorState = ES_NULL_POINTER ;

	return Local_enuErrorState ;
}
