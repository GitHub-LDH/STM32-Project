/*********************************************************************************************************************
*
* @file             ldh_driver_sr04.c
* @author           刘冬华(QQ2448544676)
* @Software         MDK V5
* @Target core      STM32F407xx
* @date             2023-01-02
* @note             接线定义：
*                   ------------------------------------
*                   模块管脚            单片机管脚
*                   Echo				PD0
					Trig				PD1
*                   电源引脚
*                   VCC                 5V电源
*                   GND                 电源地
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _ldh_driver_sr04_h_
#define _ldh_driver_sr04_h_

#include "stm32f4xx_hal.h"
#include "main.h"
#include "tim.h"
#include "usart.h"

/***********************   HC-SR04 连接引脚定义  **************************/
#define Echo_GPIO_Port                      GPIOD
#define Echo_PIN                            GPIO_PIN_0
#define Trig_GPIO_Port                      GPIOD
#define Trig_PIN                            GPIO_PIN_1

extern float ldh_distance;

float ldh_UltrasonicWave_Measure		(void);
#endif
