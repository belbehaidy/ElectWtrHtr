/*
 * TMU_config.h
 *
 *  Created on: Sep 24, 2022
 *      Author: Bassem El-Behaidy
 */

#ifndef TMU_CONFIG_H_
#define TMU_CONFIG_H_

//	Select timer channel [ TIMER0 - TIMER2 ]
#define TIMER_CHANNEL			TIMER0

//	Configure the time in ms desired to OS tick
//	Don't exceed 150 ms
#define OS_TICK					10


//	Calculate your CPU clock in KHz
#define CPU_FREQ_KHZ			( CPU_CLOCK/1000UL )


//	Assign max num of tasks in the system
//	Don't exceed 10
#define MAX_TASKS 				9

#endif /* TMU_CONFIG_H_ */
