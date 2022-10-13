/*
 * TMU_config.h
 *
 *  Created on: Sep 24, 2022
 *      Author: Bassem El-Behaidy
 */

#ifndef TMU_CONFIG_H_
#define TMU_CONFIG_H_

//select timer channel [ TIMER0 - TIMER2 ]
#define TIMER_CHANNEL			TIMER0

//configure the time in ms desired to OS tick
//don't exceed 150 ms
#define OS_TICK					10


//configure your CPU clock in KHz
#define CPU_FREQ_KHZ			( CPU_CLOCK/1000UL )


//assign max num of tasks in ur system
//don't exceed 10
#define MAX_TASKS 		9

#endif /* TMU_CONFIG_H_ */
