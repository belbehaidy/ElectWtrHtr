/*
 * LD_config.h
 *
 *  Created on: Aug 6, 2022
 *      Author: basse
 */

#ifndef _LD_CONFIG_H_
#define _LD_CONFIG_H_


typedef struct
{
	u8 LD_Grp;
	u8 LD_Pin;
}LD_t;

#define LD_MAX_NUM	1

#define LD_u8OFF	DIO_u8LOW
#define LD_u8ON		DIO_u8HIGH


/********************************************/
/*			LED 0 PIN CONFIGURATION			*/
/********************************************/
#define LD_ZERO_u8GROUP		DIO_u8GROUP_B
#define LD_ZERO_u8PIN		DIO_u8PIN0

#ifdef MASK

/********************************************/
/*			LED 1 PIN CONFIGURATION			*/
/********************************************/
#define LD_ONE_u8GROUP		DIO_u8GROUP_C
#define LD_ONE_u8PIN		DIO_u8PIN7

/********************************************/
/*			LED 2 PIN CONFIGURATION			*/
/********************************************/
#define LD_TWO_u8GROUP		DIO_u8GROUP_D
#define LD_TWO_u8PIN		DIO_u8PIN3

#endif

#endif /* _LD_CONFIG_H_ */
