/*********************************************************************************************************************
*
* @file             ldh_delay.h
* @author           Αυ¶¬»ª(QQ2448544676)
* @Software         MDK V5
* @Target core      STM32F407xx
* @date             2023-01-17
********************************************************************************************************************/

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include "tim.h"

#define DELAY_TIME htim6

void ldh_delay_us(uint16_t us);
void ldh_delay_ms(uint16_t ms);

#endif /* INC_DELAY_H_ */
