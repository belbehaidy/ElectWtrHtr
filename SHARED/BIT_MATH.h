/*
 * BIT_MATH.h
 *
 *  Created on: Sep 30, 2022
 *      Author: Bassem EL-Behaidy
 */

#ifndef BIT_MATH_H_
#define BIT_MATH_H_

#define _BIT_MASK_				0x01
#define _TWO_BITS_MASK_			0x03
#define _THREE_BITS_MASK_		0x07
#define _NIPPLE_MASK_			0x0F

#define _NIPPLE_SHIFT_			4
#define _BYTE_SHIFT_			8

#define _u8_LOW_NIPPLE_VALUE_( _u8_num_ )	( _u8_num_ & _NIPPLE_MASK_ )
#define _u8_HIGH_NIPPLE_VALUE_( _u8_num_ )	( _u8_num_ >> _NIPPLE_SHIFT_ )
#define _u8_VALUE_(bit) 		(1 << (bit))

#define _CLI_		asm volatile( " CLI " )
#define _SEI_		asm volatile( " SEI " )


#define SET_BIT( reg , bit)		reg |= _u8_VALUE_(bit)
#define CLR_BIT( reg , bit)		reg &= ~( _u8_VALUE_(bit) )
#define TOG_BIT( reg , bit)		reg ^= ( _u8_VALUE_(bit) )

#define IS_BIT( reg , bit)		( reg & _u8_VALUE_( bit ) )							/* Returns 0 if bit is Clear and Non-Zero if bit is set	*/
#define ASSIGN_BIT_VALUE( reg , bit , value )		reg = ( ( CLR_BIT( reg , bit) ) | ( ( value & _BIT_MASK_) << (bit) ) )

#define WAIT_TILL_BIT_IS_SET( reg , bit ) 	do { } while ( !(IS_BIT( reg , bit) ) )
#define WAIT_TILL_BIT_IS_CLR( reg , bit ) 	do { } while ( IS_BIT( reg , bit) )

#endif /* BIT_MATH_H_ */
