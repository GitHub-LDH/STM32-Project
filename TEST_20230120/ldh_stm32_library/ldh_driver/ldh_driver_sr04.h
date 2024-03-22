/*********************************************************************************************************************
*
* @file             ldh_driver_sr04.c
* @author           ������(QQ2448544676)
* @Software         MDK V5
* @Target core      STM32F407xx
* @date             2023-01-02
* @note             ���߶��壺
*                   ------------------------------------
*                   ģ��ܽ�            ��Ƭ���ܽ�
*                   Echo				PD0
					Trig				PD1
*                   ��Դ����
*                   VCC                 5V��Դ
*                   GND                 ��Դ��
*                   ------------------------------------
********************************************************************************************************************/

#ifndef _ldh_driver_sr04_h_
#define _ldh_driver_sr04_h_

#include "stm32f4xx_hal.h"
#include "main.h"
#include "tim.h"
#include "usart.h"

/***********************   HC-SR04 �������Ŷ���  **************************/
#define Echo_GPIO_Port                      GPIOD
#define Echo_PIN                            GPIO_PIN_0
#define Trig_GPIO_Port                      GPIOD
#define Trig_PIN                            GPIO_PIN_1

extern float ldh_distance;

float ldh_UltrasonicWave_Measure		(void);
#endif
