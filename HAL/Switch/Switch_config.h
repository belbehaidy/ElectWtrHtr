/*
 * Sw_One_config.h
 *
 *  Created on: Aug 6, 2022
 *      Author: Ahmed El-Gaafrawy
 */

#ifndef SWITCH_CONFIG_H_
#define SWITCH_CONFIG_H_


#define SWITCHES_MAX_NUM	3

typedef struct
{
	u8 sw_Grp;
	u8 sw_Pin;
	u8 sw_State;
}Switch_t;

#define SW_ZERO_u8GROUP		DIO_u8GROUP_C
#define SW_ZERO_u8PIN		DIO_u8PIN0
#define SW_ZERO_u8STATE		DIO_u8FLOAT

#define SW_ONE_u8GROUP		DIO_u8GROUP_C
#define SW_ONE_u8PIN		DIO_u8PIN1
#define SW_ONE_u8STATE		DIO_u8FLOAT

#define SW_TWO_u8GROUP		DIO_u8GROUP_C
#define SW_TWO_u8PIN		DIO_u8PIN2
#define SW_TWO_u8STATE		DIO_u8FLOAT


#endif /* SWITCH_CONFIG_H_ */
