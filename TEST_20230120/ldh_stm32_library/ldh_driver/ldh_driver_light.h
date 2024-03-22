/*********************************************************************************************************************
*
* @file             ldh_driver_light.h
* @author           刘冬华(QQ2448544676)
* @Software         MDK V5
* @Target core      STM32F407xx
* @date             2023-01-02
* @note             接线定义：
*                   ------------------------------------
*                   模块管脚            单片机管脚
*                   DAT					PA5
*                   电源引脚
*                   VCC                 3.3V电源
*                   GND                 电源地
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _ldh_driver_light_h_
#define _ldh_driver_light_h_

#include "adc.h"

/* 	所选引脚对应的ad通道 hadc1为PA5引脚  */
#define LIGHT_ADC hadc1  

extern int ldh_light;

uint8_t get_light(void);

#endif
