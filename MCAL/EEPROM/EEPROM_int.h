/*
 * EEPROM_int.h
 *
 *  Created on: Sep 17, 2022
 *      Author: basse
 */

#ifndef MCAL_EEPROM_EEPROM_INT_H_
#define MCAL_EEPROM_EEPROM_INT_H_


ES_t EEPROM_enuWriteByte( u16 Copy_u16Address , u8 Copy_u8Data );

ES_t EEPROM_enuReadByte( u16 Copy_u16Address , u8 *Copy_pu8Data );

#endif /* MCAL_EEPROM_EEPROM_INT_H_ */
