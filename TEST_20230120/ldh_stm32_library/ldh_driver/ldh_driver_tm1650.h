/*********************************************************************************************************************
*
* @file             ldh_driver_tm1650.h
* @author           ������(QQ2448544676)
* @Software         MDK V5
* @Target core      STM32F407xx
* @date             2023-01-17
* @note             ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   SDA					PD14
*					SCL					PD15
*                   ��Դ����
*                   VCC                 5V��Դ
*                   GND                 ��Դ��
*                   ------------------------------------
********************************************************************************************************************/


#ifndef INC_ldh_driver_tm1650_H_
#define INC_ldh_driver_tm1650_H_

#include "stdint.h"
#include "ldh_iic.h"

void TM1650_Wr_RAM(uint8_t Address, uint8_t Data);
void open_display(void);
void set_number(int num);
void set_number_ch(int num,char ch1,char ch2);

#endif /* INC_TM1650_H_ */
