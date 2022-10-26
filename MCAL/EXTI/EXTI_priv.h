/*
 * EXTI_priv.h

 *
 *  Created on: Aug 19, 2022
 *      Author: Bassem EL-Behaidy
 */

#ifndef MCAL_EXTI_EXTI_PRIV_H_
#define MCAL_EXTI_EXTI_PRIV_H_

#define LOW_LEVEL			55
#define ANY_LGC_LVL			56
#define	FALLING_EDGE		57
#define RISING_EDGE			58

#define	INTERRUPT			59
#define	POLLING				60

#define INT0				61
#define INT1				62
#define INT2				63


/************************************************************************************************************************/
/*										Interrupts Pins & Flags Configuration											*/
/************************************************************************************************************************/
/****************************************************/
/*					INT0 Configuration				*/
/****************************************************/
#define PIE0_BIT			6						//Peripheral Interrupt Enable Bit Number in GCIR for INT0
#define PIF0_BIT			6						//Peripheral Interrupt Flag Bit Number in GCIF for INT0
#define ISC00_BIT			0						//Interrupt Sense LSB for INT0 "ISC00" Bit Number in MCUCR Register
#define ISC01_BIT			1						//Interrupt Sense MSB for INT0 "ISC01" Bit Number in MCUCR Register
/****************************************************/
/*					INT1 Configuration				*/
/****************************************************/
#define PIE1_BIT			7						//Peripheral Interrupt Enable Bit Number in GCIR for INT1
#define PIF1_BIT			7						//Peripheral Interrupt Flag Bit Number in GCIF for INT1
#define ISC10_BIT			2						//Interrupt Sense LSB for INT1 "ISC10" Bit Number in MCUCR Register
#define ISC11_BIT			3						//Interrupt Sense MSB for INT1 "ISC11" Bit Number in MCUCR Register
/****************************************************/
/*					INT2 Configuration				*/
/****************************************************/
#define PIE2_BIT			5						//Peripheral Interrupt Enable Bit Number in GCIR for INT2
#define PIF2_BIT			5						//Peripheral Interrupt Flag Bit Number in GCIF for INT2
#define ISC2_BIT			6						//Interrupt Sense Bit for INT2 "ISC2" in MCUCSR Register

#endif /* MCAL_EXTI_EXTI_PRIV_H_ */
