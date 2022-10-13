/*
 * SevSeg_int.h
 *
 *  Created on: Aug 9, 2022
 *      Author: basse
 */

#ifndef SEVSEG_INT_H_
#define SEVSEG_INT_H_

#define DIP_ENABLED					35
#define DIP_DISABLED				39

#define MODULE_1					11
#define MODULE_2					12

#define MODULE_DISABLE				13
#define MODULE_ENABLE				14

#define DIP_OFF						DIO_u8HIGH
#define DIP_ON						DIO_u8LOW


ES_t SevSeg_enuInit(void);
ES_t SevSeg_enuSetDigitValue(u8 Copy_u8ModuleNum , u8 Copy_u8SevSegDigitValue);
ES_t SevSeg_enuSetDIPValue(u8 Copy_u8ModuleNum ,u8 Copy_u8SevSegDIPValue);
ES_t SevSeg_enuModuleControl(u8 Copy_u8ModuleNum , u8 Copy_u8SevSegModuleStatus);
ES_t SevSeg_enuFrameDelay(u8 Copy_u8SevSegTotalModules, u8 * Copy_pu8SevSegModuleDelay);


#endif /* SEVSEG_INT_H_ */
