/*
 * LD_int.h
 *
 *  Created on: Aug 6, 2022
 *      Author: basse
 */

#ifndef LD_INT_H_
#define LD_INT_H_

#define LD_u8OFF	DIO_u8LOW
#define LD_u8ON		DIO_u8HIGH


#define LD_ZERO			30
#define LD_ONE			31
#define LD_TWO			32

ES_t LD_enuInit(void);

ES_t LD_enuSetState(u8 Copy_u8LD_Num , u8 Copy_u8LDSetValue);

#endif /* LD_INT_H_ */
