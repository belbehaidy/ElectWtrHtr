/*
 * ATMEG32.h
 *
 *  Created on: Sep 30, 2022
 *      Author: Bassem El-Behaidy
 */

#ifndef ATMEG32_H_
#define ATMEG32_H_

//#include "stdTypes.h"

/************************************************
 *				Pin Type definition				*
 ************************************************//*
typedef struct
{
	u8 DIO_PinNum:4 ;
	u8 DIO_PinGrp:4 ;
}DIO_Pin_t;

#define PIN_NUM( _u8_DIO_PIN_ )				_u8_LOW_NIPPLE_VALUE_( _u8_DIO_PIN_ )
#define PIN_GRP( _u8_DIO_PIN_ )				_u8_HIGH_NIPPLE_VALUE_( _u8_DIO_PIN_ )
*/
/****************************
*		I/O Ports Macros	*
****************************/
#define _SFR_MEM_OFFSET_						0x20

#define __IO_u8_SFR__( io_addr )			*((volatile u8*) ( io_addr )
#define __IO_u16_SFR__( io_addr )			*((volatile u16*) ( io_addr )

#define __MEM_u8_SFR__( io_addr )			*((volatile u8*) ( io_addr + _SFR_MEM_OFFSET_ ))
#define __MEM_u16_SFR__( io_addr )			*((volatile u16*) ( io_addr + _SFR_MEM_OFFSET_ ))

#define __MEM_u8_ADDRESS__( mem_addr )		*((volatile u8*) mem_addr )
#define __MEM_u16_ADDRESS__( mem_addr )		*((volatile u16*) mem_addr )


/****************************
*		Port I/O Pins		*
*****************************/
#define    PIN7         7
#define    PIN6         6
#define    PIN5         5
#define    PIN4         4
#define    PIN3         3
#define    PIN2         2
#define    PIN1         1
#define    PIN0         0

/****************************
*		DIO REGISTERS		*
*****************************/
#define _SFR_PORT_A_ 							0x1B
#define IO_PORT_A			__IO_u8_SFR__( _SFR_PORT_A_ )
#define PORT_A				__MEM_u8_SFR__( _SFR_PORT_A_ )

#define _SFR_DDR_A_  							0x1A
#define IO_DDR_A			__IO_u8_SFR__( _SFR_DDR_A_ )
#define DDR_A				__MEM_u8_SFR__( _SFR_DDR_A_ )

#define _SFR_PIN_A_ 	 						0x19
#define IO_PIN_A				__IO_u8_SFR__( _SFR_PIN_A_ )
#define PIN_A					__MEM_u8_SFR__( _SFR_PIN_A_ )

#define _SFR_PORT_B_ 							0x18
#define IO_PORT_B			__IO_u8_SFR__( _SFR_PORT_B_ )
#define PORT_B				__MEM_u8_SFR__( _SFR_PORT_B_ )

#define _SFR_DDR_B_  							0x17
#define IO_DDR_B			__IO_u8_SFR__( _SFR_DDR_B_ )
#define DDR_B				__MEM_u8_SFR__( _SFR_DDR_B_ )

#define _SFR_PIN_B_ 	 						0x16
#define IO_PIN_B				__IO_u8_SFR__( _SFR_PIN_B_ )
#define PIN_B				__MEM_u8_SFR__( _SFR_PIN_B_ )

#define _SFR_PORT_C_ 							0x15
#define IO_PORT_C			__IO_u8_SFR__( _SFR_PORT_C_ )
#define PORT_C				__MEM_u8_SFR__( _SFR_PORT_C_ )

#define _SFR_DDR_C_  							0x14
#define IO_DDR_C			__IO_u8_SFR__( _SFR_DDR_C_ )
#define DDR_C				__MEM_u8_SFR__( _SFR_DDR_C_ )

#define _SFR_PIN_C_ 	 						0x13
#define IO_PIN_C			__IO_u8_SFR__( _SFR_PIN_C_ )
#define PIN_C				__MEM_u8_SFR__( _SFR_PIN_C_ )

#define _SFR_PORT_D_ 							0x12
#define IO_PORT_D			__IO_u8_SFR__( _SFR_PORT_D_ )
#define PORT_D				__MEM_u8_SFR__( _SFR_PORT_D_ )

#define _SFR_DDR_D_  							0x11
#define IO_DDR_D			__IO_u8_SFR__( _SFR_DDR_D_ )
#define DDR_D				__MEM_u8_SFR__( _SFR_DDR_D_ )

#define _SFR_PIN_D_ 	 						0x10
#define IO_PIN_D				__IO_u8_SFR__( _SFR_PIN_D_ )
#define PIN_D				__MEM_u8_SFR__( _SFR_PIN_D_ )


/********************************
*		INTERRUPT REGISTERS		*
*********************************/
#define _SFR_MCUCR_ 							0x35
#define IO_MCUCR			__IO_u8_SFR__( _SFR_MCUCR_ )
#define MCUCR				__MEM_u8_SFR__( _SFR_MCUCR_ )

#define _SFR_MCUCSR_ 							0x34
#define IO_MCUCSR			__IO_u8_SFR__( _SFR_MCUCSR_ )
#define MCUCSR				__MEM_u8_SFR__( _SFR_MCUCSR_ )

#define _SFR_GICR_ 								0x3B
#define IO_GICR				__IO_u8_SFR__( _SFR_GICR_ )
#define GICR				__MEM_u8_SFR__( _SFR_GICR_ )

#define _SFR_GIFR_ 								0x3A
#define IO_GIFR				__IO_u8_SFR__( _SFR_GIFR_ )
#define GIFR				__MEM_u8_SFR__( _SFR_GIFR_ )

/********************
 * STATUS REGISTER	*
 ********************/
#define _SFR_SREG_ 								0x3F
#define IO_SREG				__IO_u8_SFR__( _SFR_SREG_ )
#define SREG				__MEM_u8_SFR__( _SFR_SREG_ )

#define _SFR_SPMCR_ 							0x37
#define IO_SPMCR			__IO_u8_SFR__( _SFR_SPMCR_ )
#define SPMCR				__MEM_u8_SFR__( _SFR_SPMCR_ )

/************************************************
*			TIMER GENERAL RESIGTERS				*
*************************************************/

/************
 *	TIMSK	*
 ************/
#define _SFR_TIMSK_ 							0x39
#define IO_TIMSK			__IO_u8_SFR__( _SFR_TIMSK_ )
#define TIMSK				__MEM_u8_SFR__( _SFR_TIMSK_ )

/********
 * TIFR	*
 ********/
#define _SFR_TIFR_ 								0x38
#define IO_TIFR				__IO_u8_SFR__( _SFR_TIFR_ )
#define TIFR				__MEM_u8_SFR__( _SFR_TIFR_ )

/************************************************
*				TIMER0 RESIGTERS				*
*************************************************/
#define _SFR_TCCR0_ 						0x33
#define IO_TCCR0			__IO_u8_SFR__( _SFR_TCCR0_ )
#define TCCR0				__MEM_u8_SFR__( _SFR_TCCR0_ )

#define _SFR_TCNT0_ 						0x32
#define IO_TCNT0			__IO_u8_SFR__( _SFR_TCNT0_ )
#define TCNT0				__MEM_u8_SFR__( _SFR_TCNT0_ )

#define _SFR_OCR0_ 							0x3C
#define IO_OCR0				__IO_u8_SFR__( _SFR_OCR0_ )
#define OCR0				__MEM_u8_SFR__( _SFR_OCR0_ )



/************************************************
*				TIMER2 RESIGTERS				*
*************************************************/
#define _SFR_TCCR2_ 						0x25
#define IO_TCCR2			__IO_u8_SFR__( _SFR_TCCR2_ )
#define TCCR2				__MEM_u8_SFR__( _SFR_TCCR2_ )

#define _SFR_TCNT2_ 						0x24
#define IO_TCNT2			__IO_u8_SFR__( _SFR_TCCNT2_ )
#define TCNT2				__MEM_u8_SFR__( _SFR_TCNT2_ )

#define _SFR_OCR2_ 							0x23
#define IO_OCR2				__IO_u8_SFR__( _SFR_OCR2_ )
#define OCR2				__MEM_u8_SFR__( _SFR_OCR2_ )

#define _SFR_ASSR_ 							0x22
#define IO_ASSR				__IO_u8_SFR__( _SFR_ASSR_ )
#define ASSR				__MEM_u8_SFR__( _SFR_ASSR_ )


/************************************************
*				TIMER1 RESIGTERS				*
*************************************************/

#define _SFR_TCCR1A_ 						0x2F
#define IO_TCCR1A			__IO_u8_SFR__( _SFR_TCCR1A_ )
#define TCCR1A				__MEM_u8_SFR__( _SFR_TCCR1A_ )

#define _SFR_TCCR1B_ 						0x2E
#define IO_TCCR1B			__IO_u8_SFR__( _SFR_TCCR1B_ )
#define TCCR1B				__MEM_u8_SFR__( _SFR_TCCR1B_ )

#define _SFR_TCNT1H_ 						0x2D
#define IO_TCNT1H			__IO_u8_SFR__( _SFR_TCNT1H_ )
#define TCNT1H				__MEM_u8_SFR__( _SFR_TCNT1H_ )

#define _SFR_TCNT1L_ 						0x2C
#define IO_TCNT1L			__IO_u8_SFR__( _SFR_TCNT1L_ )
#define TCNT1L				__MEM_u8_SFR__( _SFR_TCNT1L_ )

#define _SFR_TCNT1_ 						0x2C
#define IO_TCNT1			__IO_u16_SFR__( _SFR_TCNT1_ )
#define TCNT1				__MEM_u16_SFR__( _SFR_TCNT1_ )

#define _SFR_OCR1AH_ 						0x2B
#define IO_OCR1AH			__IO_u8_SFR__( _SFR_OCR1AH_ )
#define OCR1AH				__MEM_u8_SFR__( _SFR_OCR1AH_ )

#define _SFR_OCR1AL_ 						0x2A
#define IO_OCR1AL			__IO_u8_SFR__( _SFR_OCR1AL_ )
#define OCR1AL				__MEM_u8_SFR__( _SFR_OCR1AL_ )

#define _SFR_OCR1A_ 						0x2A
#define IO_OCR1A			__IO_u16_SFR__( _SFR_OCR1A_ )
#define OCR1A				__MEM_u16_SFR__( _SFR_OCR1A_ )

#define _SFR_OCR1BH_ 						0x29
#define IO_OCR1BH			__IO_u8_SFR__( _SFR_OCR1BH_ )
#define OCR1BH				__MEM_u8_SFR__( _SFR_OCR1BH_ )

#define _SFR_OCR1BL_ 						0x28
#define IO_OCR1BL			__IO_u8_SFR__( _SFR_OCR1BL_ )
#define OCR1BL				__MEM_u8_SFR__( _SFR_OCR1BL_ )

#define _SFR_OCR1B_ 						0x28
#define IO_OCR1B			__IO_u16_SFR__( _SFR_OCR1B_ )
#define OCR1B				__MEM_u16_SFR__( _SFR_OCR1B_ )

#define _SFR_ICR1H_ 						0x27
#define IO_ICR1H			__IO_u8_SFR__( _SFR_ICR1H_ )
#define ICR1H				__MEM_u8_SFR__( _SFR_ICR1H_ )

#define _SFR_ICR1L_ 						0x26
#define IO_ICR1L			__IO_u8_SFR__( _SFR_ICR1L_ )
#define ICR1L				__MEM_u8_SFR__( _SFR_ICR1L_ )

#define _SFR_ICR1_ 							0x26
#define IO_ICR1				__IO_u16_SFR__( _SFR_ICR1_ )
#define ICR1				__MEM_u16_SFR__( _SFR_ICR1_ )


/********************
* ADC REGISTERS		*
*********************/
#define _SFR_ADMUX_ 						0x07
#define IO_ADMUX			__IO_u8_SFR__( _SFR_ADMUX_ )
#define ADMUX				__MEM_u8_SFR__( _SFR_ADMUX_ )

#define _SFR_ADCSRA_ 						0x06
#define IO_ADCSRA			__IO_u8_SFR__( _SFR_ADCSRA_ )
#define ADCSRA				__MEM_u8_SFR__( _SFR_ADCSRA_ )

#define _SFR_ADCH_ 							0x05
#define IO_ADCH				__IO_u8_SFR__( _SFR_ADCH_ )
#define ADCH				__MEM_u8_SFR__( _SFR_ADCH_ )

#define _SFR_ADCL_ 							0x04
#define IO_ADCL				__IO_u8_SFR__( _SFR_ADCL_ )
#define ADCL				__MEM_u8_SFR__( _SFR_ADCL_ )

#define _SFR_SFIOR_ 						0x30
#define IO_SFIOR			__IO_u8_SFR__( _SFR_SFIOR_ )
#define SFIOR				__MEM_u8_SFR__( _SFR_SFIOR_ )

/************************
*	UASRT REGISTERS		*
*************************/
#define _SFR_UDR_ 							0x0C
#define IO_UDR				__IO_u8_SFR__( _SFR_UDR_ )
#define UDR					__MEM_u8_SFR__( _SFR_UDR_ )

#define _SFR_UCSRA_ 						0x0B
#define IO_UCSRA			__IO_u8_SFR__( _SFR_UCSRA_ )
#define UCSRA				__MEM_u8_SFR__( _SFR_UCSRA_ )

#define _SFR_UCSRB_ 						0x0A
#define IO_UCSRB			__IO_u8_SFR__( _SFR_UCSRB_ )
#define UCSRB				__MEM_u8_SFR__( _SFR_UCSRB_ )

#define _SFR_UCSRC_ 						0x20
#define IO_UCSRC			__IO_u8_SFR__( _SFR_UCSRC_ )
#define UCSRC				__MEM_u8_SFR__( _SFR_UCSRC_ )

#define _SFR_UBRRL_ 						0x09
#define IO_UBRRL			__IO_u8_SFR__( _SFR_UBRRL_ )
#define UBRRL				__MEM_u8_SFR__( _SFR_UBRRL_ )

#define _SFR_UBRRH_ 						0x20
#define IO_UBRRH			__IO_u8_SFR__( _SFR_UBRRH_ )
#define UBRRH				__MEM_u8_SFR__( _SFR_UBRRH_ )

/********************
* 	SPI REGISTERS	*
*********************/
#define _SFR_SPDR_ 							0x0F
#define IO_SPDR				__IO_u8_SFR__( _SFR_SPDR_ )
#define SPDR				__MEM_u8_SFR__( _SFR_SPDR_ )

#define _SFR_SPSR_ 							0x0E
#define IO_SPSR				__IO_u8_SFR__( _SFR_SPSR_ )
#define SPSR				__MEM_u8_SFR__( _SFR_SPSR_ )

#define _SFR_SPCR_ 							0x0D
#define IO_SPCR				__IO_u8_SFR__( _SFR_SPCR_ )
#define SPCR				__MEM_u8_SFR__( _SFR_SPCR_ )

/************************
 *	EEPROM REGISTERS	*
 ************************/
#define _SFR_EEARH_ 						0x1F
#define IO_EEARH			__IO_u8_SFR__( _SFR_EEARH_ )
#define EEARH				__MEM_u8_SFR__( _SFR_EEARH_ )

#define _SFR_EEARL_ 						0x1E
#define IO_EEARL			__IO_u8_SFR__( _SFR_EEARL_ )
#define EEARL				__MEM_u8_SFR__( _SFR_EEARL_ )

#define _SFR_EEAR_ 							0x1E
#define IO_EEAR				__IO_u16_SFR__( _SFR_EEAR_ )
#define EEAR				__MEM_u16_SFR__( _SFR_EEAR_ )

#define _SFR_EEDR_ 							0x1D
#define IO_EEDR				__IO_u8_SFR__( _SFR_EEDR_ )
#define EEDR				__MEM_u8_SFR__( _SFR_EEDR_ )

#define _SFR_EECR_ 							0x1C
#define IO_EECR				__IO_u8_SFR__( _SFR_EECR_ )
#define EECR				__MEM_u8_SFR__( _SFR_EECR_ )

/************************
 *		TWI REGISTERS	*
 ************************/
#define _SFR_TWCR_ 							0x36
#define IO_TWCR				__IO_u8_SFR__( _SFR_TWCR_ )
#define TWCR				__MEM_u8_SFR__( _SFR_TWCR_ )

#define _SFR_TWDR_ 							0x03
#define IO_TWDR				__IO_u8_SFR__( _SFR_TWDR_ )
#define TWDR				__MEM_u8_SFR__( _SFR_TWDR_ )

#define _SFR_TWAR_ 							0x02
#define IO_TWAR				__IO_u8_SFR__( _SFR_TWAR_ )
#define TWAR				__MEM_u8_SFR__( _SFR_TWAR_ )

#define _SFR_TWSR_ 							0x01
#define IO_TWSR				__IO_u8_SFR__( _SFR_TWSR_ )
#define TWSR				__MEM_u8_SFR__( _SFR_TWSR_ )

#define _SFR_TWBR_ 							0x00
#define IO_TWBR				__IO_u8_SFR__( _SFR_TWBR_ )
#define TWBR				__MEM_u8_SFR__( _SFR_TWBR_ )

/************************
 *		WDT REGISTER	*
 ************************/
#define _SFR_WDTCR_ 						0x21
#define IO_WDTCR			__IO_u8_SFR__( _SFR_WDTCR_ )
#define WDTCR				__MEM_u8_SFR__( _SFR_WDTCR_ )


#endif /* ATMEG32_H_ */
