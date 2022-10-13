/*
 * WtrHtr_priv.h
 *
 *  Created on: Sep 28, 2022
 *      Author: basse
 */

#ifndef WTR_HTR_PRIV_H_
#define WTR_HTR_PRIV_H_

typedef enum { ON , OFF } OnOff_t;

#define NORMAL				5
#define SETUP				9

#define SET_ADDRESS			0x0010

#define LD_OFF				LD_u8OFF
#define LD_ON				LD_u8ON
#define LD_BLINK			19

#define TEMP_AVG_READINGS	10

#define BOUNCE_COUNTS		2
#define SETUP_COUNTS		50
#define BLINK_COUNTS		10


void DisplayTemperature(void*p);
void CheckTemperatureStatus(void*p);
void AdjustRedLampStatus(void*p);
void AdjustHeaterStatus(void*p);
void AdjustCoolentStatus(void*p);
void CheckIncrementSwitch(void*p);
void CheckDecrementSwitch(void*p);
void CheckPowerSwitch(void*p);
void EEPROM_Access( void *Copy_u16SetTempAddress );

#endif /* WtrHtr_priv.h */
