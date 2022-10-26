/*
 * EXTI_int.h

 *
 *  Created on: Aug 19, 2022
 *      Author: Bassem EL-Behaidy
 */

#ifndef MCAL_EXTI_EXTI_INT_H_
#define MCAL_EXTI_EXTI_INT_H_

#define LOW_LEVEL			55
#define ANY_LGC_LVL			56
#define	FALLING_EDGE		57
#define RISING_EDGE			58

#define	INTERRUPT			59
#define	POLLING				60

#define INT0				61
#define INT1				62
#define INT2				63

//////////////////////////////////////////////////
//		Interrupt Sense Level Choices			//
//		-----------------------------			//
//	Choice					Description			//
//////////////////////////////////////////////////
//	LOW_LEVEL		|	Low Level Sense			//
//	ANY_LGC_LVL		|	Any Logic Level Sense	//
//	FALLING_EDGE	|	Falling Edge Sense		//
//	RISING_EDGE		|	Rising Edge Sense		//
//////////////////////////////////////////////////

//////////////////////////////////////
//			Interrupt Numbers		//
//////////////////////////////////////
//	INT0	|	INT1	|	INT2	//
//////////////////////////////////////


ES_t EXTI_enuInit(void);

ES_t EXTI_enuEnableInterrupt(u8 Copy_u8IntNum);

ES_t EXTI_enuDisableInterrupt(u8 Copy_u8IntNum);

ES_t EXTI_enuSetSenseLevel(u8 Copy_u8IntNum , u8 Copy_u8SenseLevel);

ES_t EXTI_enuCallBack( u8 Copy_u8IntNum , void (*Copy_pAppFunction)(void) );

#endif /* MCAL_EXTI_EXTI_INT_H_ */
