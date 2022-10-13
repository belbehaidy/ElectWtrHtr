/*
 * TMU_int.h
 *
 *  Created on: Sep 24, 2022
 *      Author: Bassem El-Behaidy
 */

#ifndef TMU_INT_H_
#define TMU_INT_H_

#define READY 			23
#define PAUSED			56
#define KILLED			17


void TMU_vidInit(void);

void TMU_vidCreateTask( void( *Copy_pFunAppFun )( void* ) , void *Copy_pvidParameter, u8 Copy_u8Priority, u8 Copy_u8State ,u16 Copy_u16Periodicity  , u8 Copy_u8Offset);

void TMU_vidStartScheduler(void);

void TMU_vidDeleteTask(u8 Copy_u8Priority);

void TMU_vidPauseTask(u8 Copy_u8Priority);

void TMU_vidResumeTask(u8 Copy_u8Priority);


#endif /* TMU_INT_H_ */
