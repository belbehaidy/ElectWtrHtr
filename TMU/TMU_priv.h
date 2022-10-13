/*
 * TMU_priv.h
 *
 *  Created on: Sep 24, 2022
 *      Author: Bassem El-Behaidy
 */

#ifndef TMU_PRIV_H_
#define TMU_PRIV_H_

#define READY 			23
#define PAUSED			56
#define KILLED			17

typedef struct
{
	void(*pFun)(void*);
	void * parameter;
	u16 Periodicity;
	u8 state;
	u8 offset;
}TCB_t;

#endif /* TMU_PRIV_H_ */
