/*
 * EXTI_config.h

 *
 *  Created on: Aug 19, 2022
 *      Author: Bassem EL-Behaidy
 */

#ifndef MCAL_EXTI_EXTI_CONFIG_H_
#define MCAL_EXTI_EXTI_CONFIG_H_

typedef struct
{
	u8 IntNum;
	u8 SenseLvl;
	u8 IntMode;
}EXTI_t;

#define	EXTI_MAX_NUM			3

/*
 * 		INT0 Settings
 */
#define INT0_SEN_LVL			RISING_EDGE
#define INT0_STATUS				INTERRUPT

/*
 * 		INT1 Settings
 */
#define INT1_SEN_LVL			RISING_EDGE
#define INT1_STATUS				POLLING

/*
 * 		INT2 Settings
 */
#define INT2_SEN_LVL			RISING_EDGE
#define INT2_STATUS				POLLING

#endif /* MCAL_EXTI_EXTI_CONFIG_H_ */
